#ifndef FLEXSENSORREADER_H
#define FLEXSENSORREADER_H

#include <Arduino.h>

class FlexSensorReader {
public:
    FlexSensorReader(int pin1, int pin2, int pin3, int pin4); // Constructor with pin assignments
    void begin();                    // Initialize the pins
    void readSensors();              // Read all 4 flex sensor values
    int getSensor1Value();           // Return value of Sensor 1
    int getSensor2Value();           // Return value of Sensor 2
    int getSensor3Value();           // Return value of Sensor 3
    int getSensor4Value();           // Return value of Sensor 4

private:
    int flex1, flex2, flex3, flex4;  // Analog pins connected to sensors
    int _value1, _value2, _value3, _value4; // Flex sensor readings
};

#endif
