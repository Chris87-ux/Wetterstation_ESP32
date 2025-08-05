#ifndef SGP30_SENSOR_H
#define SGP30_SENSOR_H

#include "I2CSensor.h"
#include <Adafruit_SGP30.h>

// This sensor provides two values, TVOC and eCO2.
// This class will publish them as a single JSON string.
class SGP30Sensor : public I2CSensor {
public:
    SGP30Sensor(uint8_t address, const String& topic);

    void setup() override;
    void read(MQTTManager* mqttManager) override;
    String getValue() override;
    String getUnit() override;

private:
    Adafruit_SGP30 _sgp;
    uint16_t _tvoc = 0;
    uint16_t _eco2 = 0;
};

#endif // SGP30_SENSOR_H
