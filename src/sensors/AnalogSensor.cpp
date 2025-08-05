#include "AnalogSensor.h"

AnalogSensor::AnalogSensor(const String& name, int pin, const String& unit, const String& topic)
    : _name(name), _pin(pin), _unit(unit), _topic(topic), _value(0) {}

void AnalogSensor::setup() {
    pinMode(_pin, INPUT);
    _isHealthy = true; // No failure condition to check for analog read
}

void AnalogSensor::read(MQTTManager* mqttManager) {
    _value = analogRead(_pin);
    if (g_debug_mode) {
        publishDebugInfo(mqttManager);
    }
}

void AnalogSensor::publishDebugInfo(MQTTManager* mqttManager) {
    String debugTopic = getTopic() + "/debug";
    String payload = "Raw ADC: " + String(_value);
    mqttManager->publishDebug(debugTopic, payload);
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
