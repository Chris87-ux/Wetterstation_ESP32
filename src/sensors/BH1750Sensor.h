#ifndef BH1750_SENSOR_H
#define BH1750_SENSOR_H

#include "I2CSensor.h"
#include <BH1750.h>

class BH1750Sensor : public I2CSensor {
public:
    BH1750Sensor(uint8_t address, const String& topic);

    void setup() override;
    void read(MQTTManager* mqttManager) override;
    String getValue() override;
    String getUnit() override;

private:
    BH1750 _lightMeter;
    float _lux = 0.0;
};

#endif // BH1750_SENSOR_H
