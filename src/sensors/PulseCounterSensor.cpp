#include "PulseCounterSensor.h"

// Initialize static members
PulseCounterSensor* PulseCounterSensor::_instances[MAX_PULSE_SENSORS] = {nullptr};
uint8_t PulseCounterSensor::_instanceCount = 0;

// --- ISRs ---
// We need a separate ISR for each possible instance, as we can't pass context to attachInterrupt.
void IRAM_ATTR isr0() {
    if (PulseCounterSensor::_instances[0]) PulseCounterSensor::_instances[0]->handleInterrupt();
}
void IRAM_ATTR isr1() {
    if (PulseCounterSensor::_instances[1]) PulseCounterSensor::_instances[1]->handleInterrupt();
}
void IRAM_ATTR isr2() {
    if (PulseCounterSensor::_instances[2]) PulseCounterSensor::_instances[2]->handleInterrupt();
}
void IRAM_ATTR isr3() {
    if (PulseCounterSensor::_instances[3]) PulseCounterSensor::_instances[3]->handleInterrupt();
}

// Array of ISR function pointers
void (*isr_functions[MAX_PULSE_SENSORS])() = {isr0, isr1, isr2, isr3};

// --- Class Methods ---

PulseCounterSensor::PulseCounterSensor(const String& name, uint8_t pin, const String& unit, const String& topic, float conversionFactor)
    : _name(name), _pin(pin), _unit(unit), _topic(topic), _conversionFactor(conversionFactor) {
    if (_instanceCount >= MAX_PULSE_SENSORS) {
        Serial.println("ERROR: Maximum number of pulse sensors exceeded!");
        // This object will be unhealthy
    } else {
        _instances[_instanceCount] = this;
        _instanceCount++;
    }
}

void PulseCounterSensor::setup() {
    pinMode(_pin, INPUT_PULLUP); // Assuming a switch to ground, common for these sensors

    // Find the instance index to attach the correct ISR
    for (int i = 0; i < _instanceCount; i++) {
        if (_instances[i] == this) {
            attachInterrupt(digitalPinToInterrupt(_pin), isr_functions[i], FALLING);
            _isHealthy = true; // Successfully attached interrupt
            break;
        }
    }
    _lastReadTime = millis();
}

void PulseCounterSensor::read(MQTTManager* mqttManager) {
    // For rate-based sensors (like wind speed), we calculate the value here.
    // For accumulation sensors (like rain), this function does nothing,
    // as the calculation is handled externally using getAndResetPulseCount().
    if (getUnit() == "km/h") {
        if (g_debug_mode) {
            // In debug mode, we can publish the raw pulse count for wind before it's reset
            String debugTopic = getTopic() + "/debug";
            mqttManager->publishDebug(debugTopic, "Pulses this interval: " + String(_pulseCount));
        }
        unsigned long currentTime = millis();
        unsigned long timeDiff = currentTime - _lastReadTime;

        if (timeDiff > 0) {
            // Read the pulse count without resetting it for this calculation
            noInterrupts();
            unsigned long pulses = _pulseCount;
            interrupts();

            float pulsesPerSecond = (float)pulses / (timeDiff / 1000.0);
            _value = pulsesPerSecond * _conversionFactor;
        }
        _lastReadTime = currentTime;

        // Reset the pulse count after the rate is calculated for the interval
        getAndResetPulseCount();
    }
}

unsigned long PulseCounterSensor::getAndResetPulseCount() {
    noInterrupts();
    unsigned long pulses = _pulseCount;
    _pulseCount = 0;
    interrupts();
    return pulses;
}

void IRAM_ATTR PulseCounterSensor::handleInterrupt() {
    _pulseCount++;
}

String PulseCounterSensor::getName() {
    return _name;
}

String PulseCounterSensor::getValue() {
    return String(_value, 2);
}

String PulseCounterSensor::getUnit() {
    return _unit;
}

String PulseCounterSensor::getTopic() {
    return _topic;
}
