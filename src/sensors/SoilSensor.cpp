#include "SoilSensor.h"

SoilSensor::SoilSensor(uint8_t moisturePin, uint8_t tempPin, const String& topic)
    : _moisturePin(moisturePin), _topic(topic), _oneWire(tempPin), _dallasTemp(&_oneWire) {}

void SoilSensor::setup() {
    pinMode(_moisturePin, INPUT);
    _dallasTemp.begin();
    // Check if a 1-Wire device is present
    if (_dallasTemp.getDeviceCount() > 0) {
        _isHealthy = true;
    } else {
        Serial.println("No 1-Wire temperature sensor found!");
        _isHealthy = false;
    }
}

void SoilSensor::read(MQTTManager* mqttManager) {
    if (!_isHealthy) return;

    // Read analog moisture
    _moisture = analogRead(_moisturePin);

    // Read 1-Wire temperature
    _dallasTemp.requestTemperatures();
    float temp = _dallasTemp.getTempCByIndex(0);
    if (temp != DEVICE_DISCONNECTED_C) {
        _temperature = temp;
    }

    if (g_debug_mode) {
        String debugTopic = getTopic() + "/debug";
        String payload = "Raw Moisture ADC: " + String(_moisture) + ", Raw Temp: " + String(_temperature);
        mqttManager->publishDebug(debugTopic, payload);
    }
}

String SoilSensor::getValue() {
    // Return a JSON object with both values
    return "{\"moisture\":" + String(_moisture) + ",\"temperature\":" + String(_temperature, 2) + "}";
}

String SoilSensor::getUnit() {
    return "raw/%C";
}

String SoilSensor::getName() {
    return "Soil";
}

String SoilSensor::getTopic() {
    return _topic;
}
