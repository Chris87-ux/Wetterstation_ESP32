#ifndef SOIL_SENSOR_H
#define SOIL_SENSOR_H

#include "Sensor.h"
#include <OneWire.h>
#include <DallasTemperature.h>

// This composite sensor reads from an analog pin for moisture
// and a 1-Wire pin for temperature. It publishes a JSON value.
class SoilSensor : public Sensor {
public:
    SoilSensor(uint8_t moisturePin, uint8_t tempPin, const String& topic);

    void setup() override;
    void read(MQTTManager* mqttManager) override;
    String getValue() override;
    String getUnit() override;
    String getName() override;
    String getTopic() override;

private:
    uint8_t _moisturePin;
    String _topic;

    // 1-Wire members
    OneWire _oneWire;
    DallasTemperature _dallasTemp;

    // Stored values
    int _moisture = 0;
    float _temperature = 0.0;
};

#endif // SOIL_SENSOR_H
