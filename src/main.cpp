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
#include "time/TimeManager.h"
#include "calculations/RainCalculation.h"

// -- Global Objects --
HardwareSerial Serial2(2); // Use UART2 for the CO2 sensor
TimeManager timeManager(NTP_SERVER, UTC_OFFSET_SECONDS, DAYLIGHT_OFFSET_SECONDS);
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

    // Initialize Time
    timeManager.setup();

    // Initialize Sensors
    // You can add all your sensor objects here.
    // This makes it easy to extend the system.
    sensors.push_back(new AnalogSensor("LDR", LDR_PIN, "lum", LDR_TOPIC));
    sensors.push_back(new AnalogSensor("Gas", MQ2_GAS_PIN, "ppm", MQ2_TOPIC));
    sensors.push_back(new AnalogSensor("UV", GY8511_UV_PIN, "index", UV_TOPIC));
    sensors.push_back(new WindDirectionSensor("Wind Direction", WIND_DIRECTION_PIN, WIND_DIR_TOPIC));
    sensors.push_back(new PulseCounterSensor("Rain Gauge", RAIN_GAUGE_PIN, "pulses", RAIN_GAUGE_TOPIC, 0)); // No conversion here
    sensors.push_back(new PulseCounterSensor("Wind Speed", WIND_SPEED_PIN, "km/h", WIND_SPEED_TOPIC, WIND_KMH_PER_PULSE_PER_SEC));
    sensors.push_back(new BH1750Sensor(BH1750_I2C_ADDRESS, BH1750_TOPIC));
    sensors.push_back(new AS3935Sensor(AS3935_I2C_ADDRESS, AS3935_IRQ_PIN, AS3935_TOPIC));
    sensors.push_back(new MHZ19_CO2Sensor(&Serial2, MHZ19_TOPIC));

    // Setup all sensors
    for (const auto& sensor : sensors) {
        sensor->setup();
    }

    // Initialize Calculations
    // Add any calculation objects here.
    calculations.push_back(new RainCalculation(RAIN_MM_PER_PULSE, &timeManager));

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
        // Generic sensor publishing
        for (const auto& sensor : sensors) {
            mqttManager.publishData(sensor->getTopic(), sensor->getValue());
        }
        // Specific calculation publishing
        for (const auto& calc : calculations) {
            RainCalculation* rainCalc = dynamic_cast<RainCalculation*>(calc);
            if (rainCalc) {
                mqttManager.publishRain(rainCalc);
            } else {
                // Fallback for other calculation types
                mqttManager.publishData(calc->getTopic(), calc->getValue());
            }
        }
    }
}
