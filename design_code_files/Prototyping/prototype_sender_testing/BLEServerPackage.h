#ifndef BLE_SERVER_PACKAGE_H
#define BLE_SERVER_PACKAGE_H

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID        "12345678-1234-1234-1234-123456789012"
#define CHARACTERISTIC_UUID "87654321-4321-4321-4321-210987654321"

class BLEServerPackage {
  private:
    BLECharacteristic *pCharacteristic;

  public:
    BLEServerPackage(); // Constructor
    void begin();       // Initializes BLE
    void sendData(const char* messages[], uint8_t numStrings); // Send custom data
};

#endif
