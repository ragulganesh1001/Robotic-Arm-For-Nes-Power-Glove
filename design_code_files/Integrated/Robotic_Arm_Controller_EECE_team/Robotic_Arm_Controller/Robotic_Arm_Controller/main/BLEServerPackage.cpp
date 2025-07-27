#include "BLEServerPackage.h"

BLEServerPackage::BLEServerPackage() {
  // Constructor
}

void BLEServerPackage::begin() {
  Serial.begin(115200);
  BLEDevice::init("ESP32-BLE-Server");

  Serial.print("BLE Server Address: ");
  Serial.println(BLEDevice::getAddress().toString().c_str());

  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);

  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );

  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  BLEDevice::startAdvertising();
  Serial.println("BLE Server is advertising...");
}

void BLEServerPackage::sendData(const char* messages[], uint8_t numStrings) {
  // Create a buffer large enough to store all strings plus separators
  char buffer[128] = {0};

  // Convert array of strings into a single buffer (concatenated with a delimiter)
  for (uint8_t i = 0; i < numStrings; i++) {
    strcat(buffer, messages[i]); // Append string
    strcat(buffer, ",");         // Using "|" as a delimiter
  }

  // Send buffer over BLE
  pCharacteristic->setValue((uint8_t*)buffer, strlen(buffer) + 1); // +1 for null termination
  pCharacteristic->notify();

  Serial.print("Sent: ");
  Serial.println(buffer);

  delay(100); // Transmission rate --> Change as needed
}
