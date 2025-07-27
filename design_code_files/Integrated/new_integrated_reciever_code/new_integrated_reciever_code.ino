#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEClient.h>
#include <ESP32Servo.h>

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

const int servoPins[] = {2, 4, 5, 18, 19, 21, 22};

// Function to process incoming comma-separated string of integers
void processIncomingData(String data) {
  int values[7] = {0};
  int index = 0;

  // Convert String to char array
  char buffer[128];
  data.toCharArray(buffer, sizeof(buffer));

  // Tokenize input using strtok
  char* token = strtok(buffer, ",");
  while (token != nullptr && index < 7) {
    values[index] = constrain(atoi(token), 0, 180); // Clamp to 0-180 for safety
    token = strtok(nullptr, ",");
    index++;
  }

  if (index == 7) { 
    servo1.write(values[0]);
    servo2.write(values[1]);
    servo3.write(values[2]);
    servo4.write(values[3]);
    servo5.write(values[4]);
    servo6.write(values[5]);
    servo7.write(values[6]);

    Serial.print("Updated Servos: ");
    for (int i = 0; i < 7; i++) {
      Serial.print(values[i]);
      Serial.print(i < 6 ? ", " : "\n");
    }
  } else {
    Serial.println("Invalid data received (not 7 integers).");
  }
}

void setup() {
  Serial.begin(115200);

  BLEDevice::init("ESP32-BLE-Client");
  pClient = BLEDevice::createClient();

  if (pClient->connect(BLEAddress("a0:b7:65:21:70:12"))) {
    Serial.println("Connected to BLE Server.");
    BLERemoteService* pRemoteService = pClient->getService(SERVICE_UUID);
    if (pRemoteService) {
      pRemoteChar = pRemoteService->getCharacteristic(CHARACTERISTIC_UUID);
      if (pRemoteChar) {
        Serial.println("Characteristic found.");
      } else {
        Serial.println("Characteristic not found.");
      }
    } else {
      Serial.println("Service not found.");
    }
  } else {
    Serial.println("Failed to connect to BLE server.");
  }

  
  servo1.attach(servoPins[0]);
  servo2.attach(servoPins[1]);
  servo3.attach(servoPins[2]);
  servo4.attach(servoPins[3]);
  servo5.attach(servoPins[4]);
  servo6.attach(servoPins[5]);
  servo7.attach(servoPins[6]);
}

void loop() {
  if (pRemoteChar && pRemoteChar->canRead()) {
    String data = pRemoteChar->readValue();
    Serial.print("Received: ");
    Serial.println(data);
    processIncomingData(data);
  } else {
    Serial.println("Waiting for BLE data...");
  }
  delay(100);
}
