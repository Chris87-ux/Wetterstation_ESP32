#include "I2CSensor.h"

I2CSensor::I2CSensor(const String& name, uint8_t address, const String& topic)
    : _name(name), _address(address), _topic(topic) {}

void I2CSensor::setup() {
    // The main sketch should call Wire.begin()
    // This setup can be used for sensor-specific initialization
    // For example, checking if the sensor is connected.
    // Default to healthy, subclasses can override.
    _isHealthy = true;
}

String I2CSensor::getName() {
    return _name;
}

String I2CSensor::getTopic() {
    return _topic;
}

// Note: getValue(), getUnit(), and read() are not implemented here.
// They must be implemented by the concrete sensor class that inherits from I2CSensor.
// For example, a BME280 class would implement these to read temperature,
// humidity, and pressure.
