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
    if (_instanceCount < MAX_PULSE_SENSORS) {
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
            break;
        }
    }
    _lastReadTime = millis();
}

void PulseCounterSensor::read() {
    unsigned long currentTime = millis();
    unsigned long timeDiff = currentTime - _lastReadTime;

    // Atomically read and reset the pulse count
    noInterrupts();
    unsigned long pulses = _pulseCount;
    _pulseCount = 0;
    interrupts();

    if (timeDiff > 0) {
        // This calculation works for both rate (wind) and accumulation (rain)
        // For rain, the conversion factor should be "mm per pulse".
        // For wind, it should be "km/h per (pulse/sec)".
        float pulsesPerSecond = (float)pulses / (timeDiff / 1000.0);

        if (getUnit() == "mm") { // Rain gauge: total accumulation
             _value += pulses * _conversionFactor;
        } else { // Wind speed: rate
             _value = pulsesPerSecond * _conversionFactor;
        }
    }

    _lastReadTime = currentTime;
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
