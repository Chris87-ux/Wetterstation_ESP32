#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

class Sensor {
public:
    virtual ~Sensor() {}
    virtual void setup() = 0;
    virtual void read(MQTTManager* mqttManager) = 0;

    virtual bool isHealthy() const { return _isHealthy; }
    virtual String getName() = 0;       // Name of the sensor (e.g., "Temperature")
    virtual String getValue() = 0;      // Current value as a string
    virtual String getUnit() = 0;       // Unit of measurement (e.g., "°C")
    virtual String getTopic() = 0;      // MQTT topic for this sensor

protected:
    virtual void publishDebugInfo(MQTTManager* mqttManager) {
        // Base implementation does nothing
    }
    bool _isHealthy = false; // Default to unhealthy until setup() proves otherwise
};

#endif // SENSOR_H
