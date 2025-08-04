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

void SoilSensor::read() {
    if (!_isHealthy) return;

    // Read analog moisture
    _moisture = analogRead(_moisturePin);

    // Read 1-Wire temperature
    _dallasTemp.requestTemperatures();
    _temperature = _dallasTemp.getTempCByIndex(0);
    if (_temperature == DEVICE_DISCONNECTED_C) {
        Serial.println("Failed to read from DS18B20 sensor");
        _temperature = 0; // Indicate error
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
