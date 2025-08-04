#ifndef I2C_SENSOR_H
#define I2C_SENSOR_H

#include "Sensor.h"
#include <Wire.h>

class I2CSensor : public Sensor {
public:
    I2CSensor(const String& name, uint8_t address, const String& topic);

    void setup() override;
    // read() is left as a pure virtual function, as it's device-specific
    // void read() override = 0;

    String getName() override;
    String getTopic() override;

protected:
    String _name;
    uint8_t _address;
    String _topic;
};

#endif // I2C_SENSOR_H
