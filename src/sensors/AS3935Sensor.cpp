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

void AS3935Sensor::read(MQTTManager* mqttManager) {
    // Reset distance from previous readings
    _distance = 0;

    if (digitalRead(_irqPin) == HIGH) {
        int event = _lightning.readInterruptReg();
        String eventString = "";
        if (event == 1) {
            eventString = "Noise";
        } else if (event == 4) {
            eventString = "Disturber";
        } else if (event == 8) {
            eventString = "Lightning";
            _distance = _lightning.lightningDistanceKm();
        }
        if (g_debug_mode && eventString != "") {
            String debugTopic = getTopic() + "/debug";
            mqttManager->publishDebug(debugTopic, "Event: " + eventString + ", Distance: " + String(_distance) + "km");
        }
    }
}

String AS3935Sensor::getValue() {
    return String(_distance);
}

String AS3935Sensor::getUnit() {
    return "km";
}
