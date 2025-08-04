#include <Arduino.h>
#include <vector>
#include "config.h"
#include "mqtt/MQTTManager.h"
#include "sensors/Sensor.h"
#include "sensors/AnalogSensor.h"
#include "sensors/WindDirectionSensor.h"
#include "sensors/PulseCounterSensor.h"
#include "sensors/BH1750Sensor.h"
#include "sensors/AS3935Sensor.h"
#include "sensors/MHZ19_CO2Sensor.h"
// #include "sensors/YourI2CSensor.h" // Example for later
#include "calculations/Calculation.h"
#include "calculations/HeatIndex.h"

// -- Global Objects --
HardwareSerial Serial2(2); // Use UART2 for the CO2 sensor
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

    // Initialize Serial2 for CO2 Sensor
    Serial2.begin(9600, SERIAL_8N1, MHZ19_RX_PIN, MHZ19_TX_PIN);

    // Initialize MQTT
    mqttManager.setup(WIFI_SSID, WIFI_PASSWORD);

    // Initialize Sensors
    // You can add all your sensor objects here.
    // This makes it easy to extend the system.
    sensors.push_back(new AnalogSensor("LDR", ANALOG_SENSOR_PIN, "lum", "weatherstation/livingroom/ldr"));
    sensors.push_back(new AnalogSensor("Gas", MQ2_GAS_PIN, "ppm", "weatherstation/livingroom/gas"));
    sensors.push_back(new AnalogSensor("UV", GY8511_UV_PIN, "index", "weatherstation/livingroom/uv"));
    sensors.push_back(new WindDirectionSensor("Wind Direction", WIND_DIRECTION_PIN, "weatherstation/livingroom/winddir"));
    sensors.push_back(new PulseCounterSensor("Rain Gauge", RAIN_GAUGE_PIN, "mm", "weatherstation/livingroom/rain", 0.2794));
    sensors.push_back(new PulseCounterSensor("Wind Speed", WIND_SPEED_PIN, "km/h", "weatherstation/livingroom/windspeed", 2.4));
    sensors.push_back(new BH1750Sensor(BH1750_I2C_ADDRESS, "weatherstation/livingroom/light"));
    sensors.push_back(new AS3935Sensor(AS3935_I2C_ADDRESS, AS3935_IRQ_PIN, "weatherstation/livingroom/lightning"));
    sensors.push_back(new MHZ19_CO2Sensor(&Serial2, "weatherstation/livingroom/co2"));

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
