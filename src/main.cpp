#include <Arduino.h>
#include <vector>
#include "config.h"
#include "mqtt/MQTTManager.h"
#include "sensors/Sensor.h"
#include "sensors/AnalogSensor.h"
// #include "sensors/YourI2CSensor.h" // Example for later
#include "calculations/Calculation.h"
#include "calculations/HeatIndex.h"

// -- Global Objects --
MQTTManager mqttManager(MQTT_BROKER_IP, MQTT_BROKER_PORT, MQTT_CLIENT_ID);
std::vector<Sensor*> sensors;
std::vector<Calculation*> calculations;

unsigned long lastUpdateTime = 0;

// -- setup() --
// Runs once at the beginning
void setup() {
    Serial.begin(115200);
    while (!Serial) {
        ; // wait for serial port to connect.
    }
    Serial.println("Weather Station Starting...");

    // Initialize MQTT
    mqttManager.setup(WIFI_SSID, WIFI_PASSWORD);

    // Initialize Sensors
    // You can add all your sensor objects here.
    // This makes it easy to extend the system.
    sensors.push_back(new AnalogSensor("LDR", ANALOG_SENSOR_PIN, "lum", "weatherstation/livingroom/ldr"));

    // Example of how you would add a more complex I2C sensor (e.g., BME280)
    // You would need to create a BME280 class that inherits from I2CSensor
    // #include "sensors/BME280Sensor.h"
    // sensors.push_back(new BME280Sensor(BME280_I2C_ADDRESS));

    // Setup all sensors
    for (const auto& sensor : sensors) {
        sensor->setup();
    }

    // Initialize Calculations
    // Add any calculation objects here.
    calculations.push_back(new HeatIndex("weatherstation/livingroom/heatindex"));

    Serial.println("Setup complete.");
}

// -- loop() --
// Runs continuously
void loop() {
    // Keep the MQTT connection alive
    mqttManager.loop();

    // Check if it's time to read and publish data
    if (millis() - lastUpdateTime >= UPDATE_INTERVAL_MS) {
        lastUpdateTime = millis();

        Serial.println("\nReading sensor data...");
        for (const auto& sensor : sensors) {
            sensor->read();
            Serial.print(sensor->getName());
            Serial.print(": ");
            Serial.print(sensor->getValue());
            Serial.println(sensor->getUnit());
        }

        Serial.println("\nPerforming calculations...");
        for (const auto& calc : calculations) {
            calc->calculate(sensors);
            Serial.print(calc->getName());
            Serial.print(": ");
            Serial.print(calc->getValue());
            Serial.println(calc->getUnit());
        }

        Serial.println("\nPublishing data via MQTT...");
        mqttManager.publish(sensors, calculations);
    }
}
