#ifndef AS3935_SENSOR_H
#define AS3935_SENSOR_H

#include "I2CSensor.h"
#include <AS3935.h>

class AS3935Sensor : public I2CSensor {
public:
    AS3935Sensor(uint8_t address, int irqPin, const String& topic);

    void setup() override;
    void read() override;
    String getValue() override;
    String getUnit() override;

private:
    AS3935 _lightning;
    int _irqPin;
    // The value will be the distance to the storm, or 0 if no event
    int _distance = 0;
};

#endif // AS3935_SENSOR_H
