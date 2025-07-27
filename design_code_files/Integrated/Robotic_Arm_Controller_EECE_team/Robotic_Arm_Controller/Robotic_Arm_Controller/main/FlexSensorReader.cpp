#include "FlexSensorReader.h"

// Constructor: initialize sensor pins
FlexSensorReader::FlexSensorReader(int pin1, int pin2, int pin3, int pin4) {
    flex1 = pin1;
    flex2 = pin2;
    flex3 = pin3;
    flex4 = pin4;
    _value1 = _value2 = _value3 = _value4 = 0; // Initialize sensor values to 0
}

// Begin: set up pins
void FlexSensorReader::begin() {
    pinMode(flex1, INPUT);
    pinMode(flex2, INPUT);
    pinMode(flex3, INPUT);
    pinMode(flex4, INPUT);
}

// Read all 4 flex sensor values
void FlexSensorReader::readSensors() {
    _value1 = analogRead(flex1);
    _value2 = analogRead(flex2);
    _value3 = analogRead(flex3);
    _value4 = analogRead(flex4);
}

// Get value of Sensor 1
int FlexSensorReader::getSensor1Value() {
    return _value1;
}

// Get value of Sensor 2
int FlexSensorReader::getSensor2Value() {
    return _value2;
}

// Get value of Sensor 3
int FlexSensorReader::getSensor3Value() {
    return _value3;
}

// Get value of Sensor 4
int FlexSensorReader::getSensor4Value() {
    return _value4;
}
