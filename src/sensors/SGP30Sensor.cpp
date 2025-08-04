#include "SGP30Sensor.h"

SGP30Sensor::SGP30Sensor(uint8_t address, const String& topic)
    : I2CSensor("Air Quality (SGP30)", address, topic) {}

void SGP30Sensor::setup() {
    _isHealthy = _sgp.begin();
    if (!_isHealthy) {
        Serial.println("SGP30 sensor not found, check wiring!");
    }
    // For more accurate readings, a baseline can be set.
    // This would typically be saved to and restored from NVM.
    // _sgp.setIAQBaseline(0x8973, 0x8AAE);
}

void SGP30Sensor::read() {
    if (!_isHealthy) return;

    if (_sgp.IAQmeasure()) {
        _tvoc = _sgp.TVOC;
        _eco2 = _sgp.eCO2;
    } else {
        Serial.println("SGP30 measurement failed");
    }
}

String SGP30Sensor::getValue() {
    // Return a JSON object with both values
    return "{\"tvoc\":" + String(_tvoc) + ",\"eco2\":" + String(_eco2) + "}";
}

String SGP30Sensor::getUnit() {
    return "ppb/ppm";
}
