#include "WindDirectionSensor.h"

// Note: These values are examples and may need to be calibrated for your specific sensor.
// They correspond to the typical voltage dividers found in weather station wind vanes.
// The ESP32 has a 12-bit ADC, so the range is 0-4095.
const int WIND_DIR_MAP[][2] = {
    {3840, 0},    // N
    {3430, 22},   // NNE
    {3020, 45},   // NE
    {2610, 67},   // ENE
    {2200, 90},   // E
    {1790, 112},  // ESE
    {1380, 135},  // SE
    {970, 157},   // SSE
    {560, 180},   // S
    {350, 202},   // SSW
    {140, 225},   // SW
    {240, 247},   // WSW
    {450, 270},   // W
    {660, 292},   // WNW
    {870, 315},   // NW
    {1080, 337}   // NNW
};
const int NUM_DIRECTIONS = sizeof(WIND_DIR_MAP) / sizeof(WIND_DIR_MAP[0]);

WindDirectionSensor::WindDirectionSensor(const String& name, int pin, const String& topic)
    : AnalogSensor(name, pin, "deg", topic) {} // Unit is degrees, but getValue will return cardinal

String WindDirectionSensor::getValue() {
    // The raw analog value is read by the parent class's read() method
    // and stored in the _value member.
    int rawValue = AnalogSensor::getValue().toInt();
    return mapValueToDirection(rawValue);
}

const String CARDINAL_DIRECTIONS[] = {
    "N", "NNE", "NE", "ENE", "E", "ESE", "SE", "SSE",
    "S", "SSW", "SW", "WSW", "W", "WNW", "NW", "NNW"
};

String WindDirectionSensor::mapValueToDirection(int analogValue) {
    // Find the closest matching direction in the map
    int smallestDiff = 4096;
    int bestMatchIndex = 0;

    for (int i = 0; i < NUM_DIRECTIONS; i++) {
        int diff = abs(analogValue - WIND_DIR_MAP[i][0]);
        if (diff < smallestDiff) {
            smallestDiff = diff;
            bestMatchIndex = i;
        }
    }

    // Return the cardinal direction string
    return CARDINAL_DIRECTIONS[bestMatchIndex];
}

void WindDirectionSensor::publishDebugInfo(MQTTManager* mqttManager) {
    String debugTopic = getTopic() + "/debug";
    String payload = "Raw ADC: " + AnalogSensor::getValue(); // Get raw value from parent
    mqttManager->publishDebug(debugTopic, payload);
}
