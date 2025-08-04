#include "AnalogSensor.h"

AnalogSensor::AnalogSensor(const String& name, int pin, const String& unit, const String& topic)
    : _name(name), _pin(pin), _unit(unit), _topic(topic), _value(0) {}

void AnalogSensor::setup() {
    pinMode(_pin, INPUT);
    _isHealthy = true; // No failure condition to check for analog read
}

void AnalogSensor::read() {
    _value = analogRead(_pin);
}

String AnalogSensor::getName() {
    return _name;
}

String AnalogSensor::getValue() {
    return String(_value);
}

String AnalogSensor::getUnit() {
    return _unit;
}

String AnalogSensor::getTopic() {
    return _topic;
}
