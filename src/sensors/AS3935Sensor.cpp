#include "AS3935Sensor.h"

#define AS3935_INDOOR 0x12
#define AS3935_OUTDOOR 0x0E

AS3935Sensor::AS3935Sensor(uint8_t address, int irqPin, const String& topic)
    : I2CSensor("Lightning", address, topic), _lightning(address), _irqPin(irqPin) {}

void AS3935Sensor::setup() {
    pinMode(_irqPin, INPUT);
    _isHealthy = _lightning.begin();
    if (!_isHealthy) {
        Serial.println ("Lightning Sensor not started, check wiring!");
    } else {
        Serial.println ("Lightning Sensor started!");
        _lightning.setIndoorOutdoor(AS3935_OUTDOOR);
        // This value should be tuned for your specific hardware
        _lightning.setTuneCaps(0);
    }
}

void AS3935Sensor::read() {
    // Reset distance from previous readings
    _distance = 0;

    if (digitalRead(_irqPin) == HIGH) {
        int event = _lightning.readInterruptReg();
        if (event == 1) {
            Serial.println("Noise level too high, consider relocating sensor.");
        } else if (event == 4) {
            Serial.println("Disturber detected.");
        } else if (event == 8) {
            Serial.println("Lightning detected!");
            _distance = _lightning.lightningDistanceKm();
        }
    }
}

String AS3935Sensor::getValue() {
    return String(_distance);
}

String AS3935Sensor::getUnit() {
    return "km";
}
