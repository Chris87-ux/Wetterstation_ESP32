#ifndef CALCULATION_H
#define CALCULATION_H

#include "../sensors/Sensor.h"
#include <Arduino.h>
#include <vector>

class Calculation {
public:
    virtual ~Calculation() {}

    // Perform the calculation using data from the provided sensors
    virtual void calculate(const std::vector<Sensor*>& sensors) = 0;

    virtual String getName() = 0;       // Name of the calculated value (e.g., "Heat Index")
    virtual String getValue() = 0;      // The calculated value as a string
    virtual String getUnit() = 0;       // Unit of the calculated value (e.g., "°C")
    virtual String getTopic() = 0;      // MQTT topic for this calculation
};

#endif // CALCULATION_H
