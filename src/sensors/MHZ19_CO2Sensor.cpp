#include "MHZ19_CO2Sensor.h"

MHZ19_CO2Sensor::MHZ19_CO2Sensor(HardwareSerial* serial, const String& topic)
    : _topic(topic) {
    _co2_sensor.begin(serial);
}

void MHZ19_CO2Sensor::setup() {
    _co2_sensor.autoCalibration(true); // Or false if you want to calibrate manually
    Serial.println("MH-Z19 CO2 sensor initialized.");
}

void MHZ19_CO2Sensor::read() {
    int co2 = _co2_sensor.getCO2();
    if (_co2_sensor.errorCode == 0) {
        _co2_ppm = co2;
    } else {
        Serial.println("Failed to read from MH-Z19 sensor.");
        _co2_ppm = 0; // Indicate error
    }
}

String MHZ19_CO2Sensor::getValue() {
    return String(_co2_ppm);
}

String MHZ19_CO2Sensor::getUnit() {
    return "ppm";
}

String MHZ19_CO2Sensor::getName() {
    return "CO2";
}

String MHZ19_CO2Sensor::getTopic() {
    return _topic;
}
