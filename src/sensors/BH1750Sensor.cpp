#include "BH1750Sensor.h"

BH1750Sensor::BH1750Sensor(uint8_t address, const String& topic)
    : I2CSensor("Light", address, topic) {}

void BH1750Sensor::setup() {
    // The I2C Wire.begin() should be called in the main setup()
    if (!_lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, _address)) {
        Serial.println("Error initializing BH1750");
    }
}

void BH1750Sensor::read() {
    if (_lightMeter.measurementReady()) {
        _lux = _lightMeter.readLightLevel();
    }
}

String BH1750Sensor::getValue() {
    return String(_lux);
}

String BH1750Sensor::getUnit() {
    return "lx";
}
