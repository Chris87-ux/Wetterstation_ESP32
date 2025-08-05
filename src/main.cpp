#include <Arduino.h>
#include <vector>
#include <Wire.h>
#include "config.h"
#include "time/TimeManager.h"
#include "mqtt/MQTTManager.h"
#include "ota/OTAManager.h"
#include "sensors/Sensor.h"
#include "calculations/Calculation.h"
#include "calculations/RainCalculation.h"

// -- Conditionally include sensor headers --
#ifdef USE_LDR
#include "sensors/AnalogSensor.h"
#endif
#ifdef USE_MQ2_GAS
#include "sensors/AnalogSensor.h"
#endif
#ifdef USE_GY8511_UV
#include "sensors/AnalogSensor.h"
#endif
#ifdef USE_WIND_DIRECTION
#include "sensors/WindDirectionSensor.h"
#endif
#if defined(USE_RAIN_GAUGE) || defined(USE_WIND_SPEED)
#include "sensors/PulseCounterSensor.h"
#endif
#ifdef USE_BH1750_LIGHT
#include "sensors/BH1750Sensor.h"
#endif
#ifdef USE_AS3935_LIGHTNING
#include "sensors/AS3935Sensor.h"
#endif
#ifdef USE_MHZ19_CO2
#include "sensors/MHZ19_CO2Sensor.h"
#endif
#ifdef USE_SGP30_AIR_QUALITY
#include "sensors/SGP30Sensor.h"
#endif
#ifdef USE_PMS5003_AIR_QUALITY
#include "sensors/PMS5003Sensor.h"
#endif
#ifdef USE_SOIL_SENSOR
#include "sensors/SoilSensor.h"
#endif

// -- Global Objects --
bool g_debug_mode = false; // Global debug flag
TimeManager timeManager(NTP_SERVER, UTC_OFFSET_SECONDS, DAYLIGHT_OFFSET_SECONDS);
MQTTManager mqttManager(MQTT_BROKER_IP, MQTT_BROKER_PORT, MQTT_CLIENT_ID);
OTAManager otaManager(OTA_HOSTNAME);
RainCalculation rainCalc(RAIN_MM_PER_PULSE, &timeManager);

// -- Conditionally declare sensor objects --
#ifdef USE_LDR
AnalogSensor ldr("LDR", LDR_PIN, "lum", LDR_TOPIC);
#endif
#ifdef USE_MQ2_GAS
AnalogSensor gasSensor("Gas", MQ2_GAS_PIN, "ppm", MQ2_TOPIC);
#endif
#ifdef USE_GY8511_UV
AnalogSensor uvSensor("UV", GY8511_UV_PIN, "index", UV_TOPIC);
#endif
#ifdef USE_WIND_DIRECTION
WindDirectionSensor windDirSensor("Wind Direction", WIND_DIRECTION_PIN, WIND_DIR_TOPIC);
#endif
#ifdef USE_RAIN_GAUGE
PulseCounterSensor rainGauge("Rain Gauge", RAIN_GAUGE_PIN, "pulses", RAIN_GAUGE_TOPIC, 0);
#endif
#ifdef USE_WIND_SPEED
PulseCounterSensor windSpeedSensor("Wind Speed", WIND_SPEED_PIN, "km/h", WIND_SPEED_TOPIC, WIND_KMH_PER_PULSE_PER_SEC);
#endif
#ifdef USE_BH1750_LIGHT
BH1750Sensor lightSensor(BH1750_I2C_ADDRESS, BH1750_TOPIC);
#endif
#ifdef USE_AS3935_LIGHTNING
AS3935Sensor lightningSensor(AS3935_I2C_ADDRESS, AS3935_IRQ_PIN, AS3935_TOPIC);
#endif
#ifdef USE_MHZ19_CO2
HardwareSerial Serial2(2);
MHZ19_CO2Sensor co2Sensor(&Serial2, MHZ19_TOPIC);
#endif
#ifdef USE_SGP30_AIR_QUALITY
SGP30Sensor sgp30Sensor(SGP30_I2C_ADDRESS, SGP30_TOPIC);
#endif
#ifdef USE_PMS5003_AIR_QUALITY
HardwareSerial Serial1(1);
PMS5003Sensor pms5003Sensor(&Serial1, PMS5003_TOPIC);
#endif
#ifdef USE_SOIL_SENSOR
SoilSensor soilSensor(SOIL_MOISTURE_PIN, SOIL_TEMP_PIN, SOIL_TOPIC);
#endif


// Vectors to hold references to the objects
std::vector<Sensor*> sensors;
std::vector<Calculation*> calculations;

unsigned long lastUpdateTime = 0;

// -- setup() --
// Runs once at the beginning
void setup() {
    Serial.begin(115200);
    while (!Serial) { ; }
    Serial.println("Weather Station Starting...");

    // Initialize hardware buses
    Wire.begin();
    #ifdef USE_MHZ19_CO2
    Serial2.begin(9600, SERIAL_8N1, MHZ19_RX_PIN, MHZ19_TX_PIN);
    #endif
    #ifdef USE_PMS5003_AIR_QUALITY
    Serial1.begin(9600, SERIAL_8N1, PMS5003_RX_PIN, PMS5003_TX_PIN);
    #endif

    // Initialize network services
    mqttManager.setup(WIFI_SSID, WIFI_PASSWORD);
    timeManager.setup();
    otaManager.setup();

    // Populate the sensor and calculation vectors
    #ifdef USE_LDR
    sensors.push_back(&ldr);
    #endif
    #ifdef USE_MQ2_GAS
    sensors.push_back(&gasSensor);
    #endif
    #ifdef USE_GY8511_UV
    sensors.push_back(&uvSensor);
    #endif
    #ifdef USE_WIND_DIRECTION
    sensors.push_back(&windDirSensor);
    #endif
    #ifdef USE_RAIN_GAUGE
    sensors.push_back(&rainGauge);
    #endif
    #ifdef USE_WIND_SPEED
    sensors.push_back(&windSpeedSensor);
    #endif
    #ifdef USE_BH1750_LIGHT
    sensors.push_back(&lightSensor);
    #endif
    #ifdef USE_AS3935_LIGHTNING
    sensors.push_back(&lightningSensor);
    #endif
    #ifdef USE_MHZ19_CO2
    sensors.push_back(&co2Sensor);
    #endif
    #ifdef USE_SGP30_AIR_QUALITY
    sensors.push_back(&sgp30Sensor);
    #endif
    #ifdef USE_PMS5003_AIR_QUALITY
    sensors.push_back(&pms5003Sensor);
    #endif
    #ifdef USE_SOIL_SENSOR
    sensors.push_back(&soilSensor);
    #endif

    calculations.push_back(&rainCalc);

    // Setup all sensors
    for (const auto& sensor : sensors) {
        sensor->setup();
    }

    // Report health status of all sensors
    Serial.println("\n--- Sensor Health Check ---");
    for (const auto& sensor : sensors) {
        bool isHealthy = sensor->isHealthy();
        Serial.print(sensor->getName());
        Serial.print(": ");
        Serial.println(isHealthy ? "OK" : "FAIL");
        mqttManager.publishHealthStatus(sensor);
    }
    Serial.println("---------------------------");

    Serial.println("\nSetup complete.");
}

// -- loop() --
// Runs continuously
void loop() {
    otaManager.loop();
    // Keep the MQTT connection alive
    mqttManager.loop();

    // Check if it's time to read and publish data
    if (millis() - lastUpdateTime >= UPDATE_INTERVAL_MS) {
        lastUpdateTime = millis();

        Serial.println("\nReading sensor data...");
        for (const auto& sensor : sensors) {
            sensor->read(&mqttManager);
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
