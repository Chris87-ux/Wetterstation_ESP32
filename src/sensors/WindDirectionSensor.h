#ifndef WIND_DIRECTION_SENSOR_H
#define WIND_DIRECTION_SENSOR_H

#include "AnalogSensor.h"

class WindDirectionSensor : public AnalogSensor {
public:
    WindDirectionSensor(const String& name, int pin, const String& topic);

    // Override getValue to provide a cardinal direction
    String getValue() override;

protected:
    void publishDebugInfo(MQTTManager* mqttManager) override;

private:
    // Helper to map analog value to direction
    String mapValueToDirection(int analogValue);
};

#endif // WIND_DIRECTION_SENSOR_H
