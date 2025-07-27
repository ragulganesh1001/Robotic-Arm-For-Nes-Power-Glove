#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEClient.h>
#include <ESP32Servo.h>

#define SERVICE_UUID        "12345678-1234-1234-1234-123456789012"
#define CHARACTERISTIC_UUID "87654321-4321-4321-4321-210987654321"

#define NUM_LEDS 12
#define NUM_FLEX 4

// Threshold values for flex sensors
#define NOT_CONNECTED 100 // turn LEDs off when no flex sensors are connected
int fully_curled[NUM_FLEX] = {400, 390, 350, 380};
int partial_curl[NUM_FLEX] = {530, 480, 450, 500};
int slight_bend[NUM_FLEX] = {680, 540, 610, 650};
/*
T 800 600 450
I 530 450 350
M 700 550 350
R 700 530 400
*/

// Enum for LED names
// enum LedName { LED_T1, LED_T2, LED_T3, LED_I1, LED_I2, LED_I3, LED_M1, LED_M2, LED_M3, LED_R1, LED_R2, LED_R3 };

// Array to store LED pin mappings
int ledPins[NUM_LEDS] = {14, 12, 13, 25, 26, 27, 21, 22, 23, 5, 18, 19};

// Create servo objects
Servo Rservo;  
Servo Pservo;
Servo Yservo;

#define PSERVO_PIN 17  // Define the pin for Pitch Servo
#define YSERVO_PIN 16  // Define the pin for Yaw Servo
#define RSERVO_PIN 4  // Define the pin for Roll Servo

BLERemoteCharacteristic* pRemoteChar;
BLEClient* pClient;

void full_curl(int index){
  digitalWrite(ledPins[index], HIGH);
  digitalWrite(ledPins[index + 1], HIGH);
  digitalWrite(ledPins[index + 2], HIGH);
}

void partial_curling(int index){
  digitalWrite(ledPins[index], HIGH);
  digitalWrite(ledPins[index + 1], HIGH);
  digitalWrite(ledPins[index + 2], LOW);
}

void slight_curl(int index){
  digitalWrite(ledPins[index], HIGH);
  digitalWrite(ledPins[index + 1], LOW);
  digitalWrite(ledPins[index + 2], LOW);
}

void no_connection(int index){
  digitalWrite(ledPins[index], LOW);
  digitalWrite(ledPins[index + 1], LOW);
  digitalWrite(ledPins[index + 2], LOW);
}

void controlLEDs(int flexValue, int startIndex, int *fully_curled, int *partial_curl, int *slight_bend) {
    // Control which LEDs are lit up for each finger
    int LEDindex = startIndex * 3;

    // Check which finger is being measured
    // 0: thumb
    // 1: index
    // 2: middle
    // 3: ring
    switch(startIndex){
      case 0:
        if(flexValue < 0){
          no_connection(LEDindex);
        }
        else if(flexValue <= fully_curled[startIndex]){
          full_curl(LEDindex);
        }
        else if(flexValue <= partial_curl[startIndex]){
          partial_curling(LEDindex);
        }
        else if(flexValue <= slight_bend[startIndex]){
          slight_curl(LEDindex);
        }
        else{
          no_connection(LEDindex);
        }
        break;

      case 1:
        if(flexValue < 0){
          no_connection(LEDindex);
        }
        else if(flexValue <= fully_curled[startIndex]){
          full_curl(LEDindex);
        }
        else if(flexValue <= partial_curl[startIndex]){
          partial_curling(LEDindex);
        }
        else if(flexValue <= slight_bend[startIndex]){
          slight_curl(LEDindex);
        }
        else{
          no_connection(LEDindex);
        }
        break;

      case 2:
        if(flexValue < 0){
          no_connection(LEDindex);
        }
        else if(flexValue <= fully_curled[startIndex]){
          full_curl(LEDindex);
        }
        else if(flexValue <= partial_curl[startIndex]){
          partial_curling(LEDindex);
        }
        else if(flexValue <= slight_bend[startIndex]){
          slight_curl(LEDindex);
        }
        else{
          no_connection(LEDindex);
        }
        break;

      case 3:
        if(flexValue < 0){
          no_connection(LEDindex);
        }
        else if(flexValue <= fully_curled[startIndex]){
          full_curl(LEDindex);
        }
        else if(flexValue <= partial_curl[startIndex]){
          partial_curling(LEDindex);
        }
        else if(flexValue <= slight_bend[startIndex]){
          slight_curl(LEDindex);
        }
        else{
          no_connection(LEDindex);
        }
        break;
      default:
      // 526 in binary 0010 0000 1110
      digitalWrite(ledPins[0], HIGH);
      digitalWrite(ledPins[3], HIGH);
      digitalWrite(ledPins[4], HIGH);
      digitalWrite(ledPins[8], HIGH);
      break;
    }
}

void reconnectBLE() {
  static int retryCount = 0;
  const int maxRetries = 2;

  if (!pClient->isConnected()) {
    Serial.println("Disconnected from BLE. Attempting to reconnect...");

    pClient->disconnect();  // Clean up previous connection
    delay(100);
    retryCount = 0;

    while (!pClient->connect(BLEAddress("90:15:06:94:1b:fa")) && retryCount < maxRetries) {
      retryCount++;
      Serial.print("Retry attempt ");
      Serial.println(retryCount);
      delay(100);
    }

    if (pClient->isConnected()) {
      Serial.println("Reconnected to BLE Server!");
      BLERemoteService* pRemoteService = pClient->getService(SERVICE_UUID);
      if (pRemoteService) {
        pRemoteChar = pRemoteService->getCharacteristic(CHARACTERISTIC_UUID);
        if (pRemoteChar) {
          Serial.println("Characteristic found after reconnection.");
        } else {
          Serial.println("Characteristic not found after reconnection.");
        }
      } else {
        Serial.println("Service not found after reconnection.");
      }
    } else {
      Serial.println("Failed to reconnect after maximum retries.");
      //delay(100);
      Serial.println("Restarting ESP32...");
      //delay(100);
      ESP.restart();  // Reset the ESP32
    }
  }
}



void setup() 
{
  Serial.begin(115200);
  
  // Set LEDs as output
  for (int i = 0; i < NUM_LEDS; i++) 
  {
    pinMode(ledPins[i], OUTPUT); // Set each LED pin as an output
  }

  Rservo.attach(RSERVO_PIN);  // Attach Servo 1 to pin 4 
  Pservo.attach(PSERVO_PIN);  // Attach Servo 2 to pin 17
  Yservo.attach(YSERVO_PIN);  // Attach Servo 3 to pin 16

  BLEDevice::init("ESP32-BLE-Client");

  pClient = BLEDevice::createClient();

  if (pClient->connect(BLEAddress("90:15:06:94:1b:fa"))) {  // Replace with actual BLE server address
    Serial.println("Connected to BLE Server!");
    BLERemoteService* pRemoteService = pClient->getService(SERVICE_UUID);
    if (pRemoteService) {
      pRemoteChar = pRemoteService->getCharacteristic(CHARACTERISTIC_UUID);
      if (pRemoteChar) {
        Serial.println("Characteristic found, waiting for data...");
      } else {
        Serial.println("Characteristic not found!");
      }
    } else {
      Serial.println("Service not found!");
    }
  } else {
    Serial.println("Failed to connect to BLE server!");
  }
}

void loop() 
{
  float roll, pitch, yaw;
  int rollServo, pitchServo, yawServo = 0;
  static int prevRollServo, prevPitchServo, prevYawServo = 0;
  int flexSensors[4] = {0, 0, 0, 0};

  if (pClient->isConnected() && pRemoteChar && pRemoteChar->canRead()) 
  {
    String value = pRemoteChar->readValue(); // Use Arduino String

    if (value.length() > 0 && value.indexOf(',') != -1) {
      char receivedBuffer[128];
      strcpy(receivedBuffer, value.c_str()); // Convert String to char array

      if (sscanf(receivedBuffer, "%d,%d,%d,%d,%f,%f,%f,", 
          &flexSensors[0], &flexSensors[1], &flexSensors[2], &flexSensors[3],
          &roll, &pitch, &yaw) == 7) 
      {
        Serial.print("Received Data: ");
        Serial.println(receivedBuffer);

        for (int i = 0; i < 4; i++) {
          controlLEDs(flexSensors[i], i, fully_curled, partial_curl, slight_bend);
        }

        rollServo = map(roll, -90, 90, 0, 180);
        pitchServo = map(pitch, -90, 90, 0, 180);
        yawServo = map(yaw, -50, 50, 0, 180);

        Serial.println(rollServo);
        Serial.println(pitchServo);
        Serial.println(yawServo);

        // Filter for motors
        if (abs(prevRollServo - rollServo) > 2) {
          Rservo.write(rollServo);
          prevRollServo = rollServo;
        }
        if (abs(prevPitchServo - pitchServo) > 2) {
          Pservo.write(pitchServo);
          prevPitchServo = pitchServo;
        }
        if (abs(prevYawServo - yawServo) > 2) {
          Yservo.write(yawServo);
          prevYawServo = yawServo;
        }
      } 
      else {
        Serial.println("Parsing error: Check incoming data format!");
      }
    } 
    else {
      Serial.println("No valid data received or empty string!");
    }
  }
  else {
    reconnectBLE();
  }

  delay(50);
}
