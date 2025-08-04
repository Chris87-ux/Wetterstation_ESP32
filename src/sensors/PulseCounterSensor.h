#ifndef PULSE_COUNTER_SENSOR_H
#define PULSE_COUNTER_SENSOR_H

#include "Sensor.h"
#include <Arduino.h>

// This class is designed to handle multiple pulse-counting sensors by using a static ISR
// and a static array of object pointers. This is a common pattern for C++ on embedded systems.
#define MAX_PULSE_SENSORS 4 // Maximum number of pulse sensors supported

class PulseCounterSensor : public Sensor {
public:
    PulseCounterSensor(const String& name, uint8_t pin, const String& unit, const String& topic, float conversionFactor);

    void setup() override;
    void read() override;

    String getName() override;
    String getValue() override;
    String getUnit() override;
    String getTopic() override;

    // The public ISR handler that will be called by the actual ISR
    void handleInterrupt();

    // Get the number of pulses since the last call and reset the counter
    unsigned long getAndResetPulseCount();

private:
    String _name;
    uint8_t _pin;
    String _unit;
    String _topic;
    float _conversionFactor;
    float _value = 0.0;

    // Volatile is crucial here because this variable is modified by an ISR
    // and read in the main loop. This prevents the compiler from optimizing
    // away reads of this variable.
    volatile unsigned long _pulseCount = 0;

    unsigned long _lastReadTime = 0;

    // Static members to allow the ISR to find the correct object instance
    static PulseCounterSensor* _instances[MAX_PULSE_SENSORS];
    static uint8_t _instanceCount;
};

#endif // PULSE_COUNTER_SENSOR_H
