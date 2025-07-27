#include "BLEServerPackage.h"
#include "ICM20948_Filter.h"
#include "FlexSensorReader.h"

//Define pins for Flex Sensors
#define Flex_Pin_1 33 //GPIO 36 (SVP)
#define Flex_Pin_2 34 //GPIO 36 (SVN)
#define Flex_Pin_3 35 //GPIO 36
#define Flex_Pin_4 25 //GPIO 36

ICM20948Filter icmFilter;
FlexSensorReader flexReader(Flex_Pin_1, Flex_Pin_2, Flex_Pin_3, Flex_Pin_4);
BLEServerPackage bleServer;

void setup() {
  flexReader.begin();

  bleServer.begin(); // Initialize BLE Server

  Serial.begin(115200);
  if(!icmFilter.begin()) {
    Serial.println("ICM initialization failed!");
    while(1);
  }
  Serial.println("ICM initialization success!");
}

void loop() {
  icmFilter.update(); //update MPU6050 sensor data
  flexReader.readSensors();
  
  //Read roll, pitch, and yaw
  float roll = icmFilter.getRoll();
  float pitch = icmFilter.getPitch();
  float yaw = icmFilter.getYaw();

  //Convert roll, pitch, and yaw to string values
  String roll_str = String(roll);
  String pitch_str = String(pitch);
  String yaw_str = String(yaw);

  //Read Flex Sensors Values
  int sensor1 = flexReader.getSensor1Value();
  int sensor2 = flexReader.getSensor2Value();
  int sensor3 = flexReader.getSensor3Value();
  int sensor4 = flexReader.getSensor4Value();

  //Convert Flex Sensor Values to String
  String sensor1_str = String(sensor1);
  String sensor2_str = String(sensor2);
  String sensor3_str = String(sensor3);
  String sensor4_str = String(sensor4);

  // Define the array of values dynamically in main
  const char* messages[] = { sensor1_str.c_str(), sensor2_str.c_str(), sensor3_str.c_str(), sensor4_str.c_str(), roll_str.c_str(), pitch_str.c_str(), yaw_str.c_str() }; //converting string to constant char values
  uint8_t numStrings = sizeof(messages) / sizeof(messages[0]);

  // Send custom data
  bleServer.sendData(messages, numStrings);

  delay(80); // delay before sending next data
}
