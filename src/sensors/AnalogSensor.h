#ifndef ANALOG_SENSOR_H
#define ANALOG_SENSOR_H

#include "Sensor.h"

class AnalogSensor : public Sensor {
public:
    AnalogSensor(const String& name, int pin, const String& unit, const String& topic);

    void setup() override;
    void read(MQTTManager* mqttManager) override;

    String getName() override;
    String getValue() override;
    String getUnit() override;
    String getTopic() override;

private:
    String _name;
    int _pin;
    String _unit;
    String _topic;
    int _value;
};

#endif // ANALOG_SENSOR_H
