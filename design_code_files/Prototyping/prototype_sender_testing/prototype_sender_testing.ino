#include "BLEServerPackage.h"

BLEServerPackage bleServer;

void setup() {
  Serial.begin(115200);
  bleServer.begin(); 
}

void loop() {
  static int generalVal = 0;
  static bool generalIncreasing = true;

  static int midVal = 80;
  static bool midIncreasing = true;

  // Build comma-separated string
  String dataString = "";
  for (int i = 0; i < 7; i++) {
    if (i == 5) {
      dataString += String(midVal); // 6th value: 80↔110
    } else {
      dataString += String(generalVal); // All others: 0↔180
    }

    if (i < 6) dataString += ",";
  }

  // Convert to char buffer
  char messageBuffer[128];
  dataString.toCharArray(messageBuffer, sizeof(messageBuffer));
  messageBuffer[sizeof(messageBuffer) - 1] = '\0';

  // Send via BLE
  const char* messages[] = { messageBuffer };
  uint8_t numStrings = sizeof(messages) / sizeof(messages[0]);
  bleServer.sendData(messages, numStrings);

  Serial.print("Sent: ");
  Serial.println(messageBuffer);

  // === Update general value (0 ↔ 180)
  if (generalIncreasing) {
    generalVal++;
    if (generalVal >= 180) generalIncreasing = false;
  } else {
    generalVal--;
    if (generalVal <= 0) generalIncreasing = true;
  }

  // === Update mid value (80 ↔ 110)
  if (midIncreasing) {
    midVal++;
    if (midVal >= 110) midIncreasing = false;
  } else {
    midVal--;
    if (midVal <= 80) midIncreasing = true;
  }

  delay(10);  //delay for update rate
}
