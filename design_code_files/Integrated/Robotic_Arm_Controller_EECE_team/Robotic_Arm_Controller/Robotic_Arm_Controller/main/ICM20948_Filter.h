#ifndef ICM20948Filter_H
#define ICM20948Filter_H

#include <Adafruit_ICM20948.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

class ICM20948Filter {
public:
    ICM20948Filter();
    bool begin();
    void update();
    float getPitch() const;
    float getRoll() const;
    float getYaw() const;

private:
    Adafruit_ICM20948 icm;
    float pitch, roll, yaw;

    void calculateOrientation(sensors_event_t& accel, sensors_event_t& gyro);
};

#endif // ICM20948HANDLER_H