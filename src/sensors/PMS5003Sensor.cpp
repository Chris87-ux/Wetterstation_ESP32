#include "PMS5003Sensor.h"

PMS5003Sensor::PMS5003Sensor(HardwareSerial* serial, const String& topic)
    : _serial(serial), _topic(topic) {}

void PMS5003Sensor::setup() {
    _isHealthy = _aqi.begin_UART(_serial);
    if (!_isHealthy) {
        Serial.println("Could not find PM2.5 sensor, check wiring!");
    }
}

void PMS5003Sensor::read() {
    if (!_isHealthy) return;

    if (!_aqi.read(&_data)) {
        Serial.println("Could not read from PM2.5 sensor");
    }
}

String PMS5003Sensor::getValue() {
    // Return a JSON object with the main values
    return "{\"pm10\":" + String(_data.pm10_standard) +
           ",\"pm25\":" + String(_data.pm25_standard) +
           ",\"pm100\":" + String(_data.pm100_standard) + "}";
}

String PMS5003Sensor::getUnit() {
    return "ug/m3";
}

String PMS5003Sensor::getName() {
    return "Air Quality (PMS5003)";
}

String PMS5003Sensor::getTopic() {
    return _topic;
}
