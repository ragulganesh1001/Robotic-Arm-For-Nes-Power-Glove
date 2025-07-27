#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEClient.h>
#include <ESP32Servo.h>  // Ensure you use the ESP32-compatible Servo library

// ***** BLE Configuration *****
#define SERVICE_UUID        "12345678-1234-1234-1234-123456789012"
#define CHARACTERISTIC_UUID "87654321-4321-4321-4321-210987654321"

BLEClient* pClient = nullptr;
BLERemoteCharacteristic* pRemoteChar = nullptr;

// ***** Servo Configuration *****
Servo servo1; // Thumb
Servo servo2; // Pointer
Servo servo3; // Middle
Servo servo4; // Ring
Servo servo5; // Roll
Servo servo6; // Yaw
Servo servo7; // Pitch

// Define servo pins with a brace-enclosed initializer
const int servoPins[] = {2, 4, 5, 18, 19, 21, 22};

// Forward declaration of processIncomingData
void processIncomingData(String data);

void setup() {
  Serial.begin(115200);

  // Initialize BLE client
  BLEDevice::init("ESP32-BLE-Client");
  pClient = BLEDevice::createClient();

  // Attempt connection to the BLE server (replace with your server's MAC address)
  if (pClient->connect(BLEAddress("a0:b7:65:21:70:12"))) {
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

  // Initialize servos by attaching them to their respective pins
  servo1.attach(servoPins[0]);//thumb
  servo2.attach(servoPins[1]);//index
  servo3.attach(servoPins[2]);//middle
  servo4.attach(servoPins[3]);//ring
  servo5.attach(servoPins[4]);//wrist
  servo6.attach(servoPins[5]);//pitch
  servo7.attach(servoPins[6]);//yaw
}

void loop() {
  if (pRemoteChar && pRemoteChar->canRead()) {
    String data = pRemoteChar->readValue();
    Serial.print("Received Data: ");
    Serial.println(data);
    processIncomingData(data);
  } else {
    Serial.println("Waiting for data...");
  }
  delay(1);
}

// Function to process the incoming data string and update servo positions
void processIncomingData(String data) {
  int values[7] = {0};  // Array to store parsed servo positions
  int index = 0;
  
  // Copy the String data into a character array buffer
  char dataArray[128];
  data.toCharArray(dataArray, sizeof(dataArray));
  
  // Parse comma-separated values using strtok
  char* token = strtok(dataArray, ",");
  while (token != NULL && index < 7) {
    values[index] = atoi(token);
    token = strtok(NULL, ",");
    index++;
  }
  
  // Update servo positions (assuming each value is within 0-180 degrees)
  servo1.write(values[0]);
  servo2.write(values[1]);
  servo3.write(values[2]);
  servo4.write(values[3]);
  servo5.write(values[4]);
  servo6.write(values[5]);
  servo7.write(values[6]);
  
  Serial.println("Servos Updated!");
}