#include "HeatIndex.h"

HeatIndex::HeatIndex(const String& topic) : _topic(topic) {}

void HeatIndex::calculate(const std::vector<Sensor*>& sensors) {
    float tempC = -999;
    float humidity = -999;

    // Find the temperature and humidity sensors
    for (const auto& sensor : sensors) {
        if (sensor->getName() == "Temperature") {
            tempC = sensor->getValue().toFloat();
        }
        if (sensor->getName() == "Humidity") {
            humidity = sensor->getValue().toFloat();
        }
    }

    // Only calculate if we found both values
    if (tempC != -999 && humidity != -999) {
        _heatIndex = computeHeatIndex(tempC, humidity);
    } else {
        _heatIndex = -999; // Indicate an error or missing data
    }
}

String HeatIndex::getName() {
    return "Heat Index";
}

String HeatIndex::getValue() {
    return String(_heatIndex, 2); // Format to 2 decimal places
}

String HeatIndex::getUnit() {
    return "°C";
}

String HeatIndex::getTopic() {
    return _topic;
}

// Using the Steadman formula for heat index in Celsius
// HI = T - 0.55 * (1 - 0.01 * RH) * (T - 14.5)
float HeatIndex::computeHeatIndex(float temperature, float humidity) {
    if (temperature < 26.7) { // Formula is generally used for temps >= 27°C
        return temperature;
    }

    float hi = temperature - 0.55 * (1.0 - 0.01 * humidity) * (temperature - 14.5);
    return hi;
}
