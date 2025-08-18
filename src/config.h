#ifndef CONFIG_H
#define CONFIG_H

// -- WIFI --
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

// -- MQTT --
#define MQTT_BROKER_IP "192.168.1.100"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "ESP32WeatherStation"
#define MQTT_USER ""
#define MQTT_PASSWORD ""

// -- SENSOR CONFIGURATION --

// --- Enable/Disable Sensors ---
// Comment out a line to disable the sensor and exclude it from the build.
#define USE_LDR
#define USE_MQ2_GAS
#define USE_GY8511_UV
#define USE_WIND_DIRECTION
#define USE_RAIN_GAUGE
#define USE_WIND_SPEED
#define USE_BH1750_LIGHT
#define USE_AS3935_LIGHTNING
#define USE_MHZ19_CO2
// #define USE_SGP30_AIR_QUALITY
// #define USE_PMS5003_AIR_QUALITY
// #define USE_SOIL_SENSOR
// #define USE_GEIGER_COUNTER

// -- LDR (example) --
#define LDR_PIN 34
#define LDR_TOPIC "weatherstation/ldr"

// -- MQ-2 Gas Sensor --
#define MQ2_GAS_PIN 32
#define MQ2_TOPIC "weatherstation/gas"

// -- GY-8511 UV Sensor --
#define GY8511_UV_PIN 33
#define UV_TOPIC "weatherstation/uv"

// -- Wind Direction Sensor (WH-SP-WD) --
#define WIND_DIRECTION_PIN 35
#define WIND_DIR_TOPIC "weatherstation/winddir"

// -- Rain Gauge (MS-WH-SP-RG) --
#define RAIN_GAUGE_PIN 26
#define RAIN_GAUGE_TOPIC "weatherstation/rain"
#define RAIN_MM_PER_PULSE 0.2794 // Amount of rain per pulse/tip

// -- Wind Speed Sensor (WH-SP-WS01) --
#define WIND_SPEED_PIN 27
#define WIND_SPEED_TOPIC "weatherstation/windspeed"
#define WIND_KMH_PER_PULSE_PER_SEC 2.4 // Factor to convert (pulses/sec) to km/h

// -- BH1750 Light Sensor --
#define BH1750_I2C_ADDRESS 0x23
#define BH1750_TOPIC "weatherstation/light"

// -- AS3935 Lightning Sensor --
#define AS3935_I2C_ADDRESS 0x03
#define AS3935_IRQ_PIN 4
#define AS3935_TOPIC "weatherstation/lightning"

// -- MH-Z19E CO2 Sensor --
#define MHZ19_RX_PIN 16
#define MHZ19_TX_PIN 17
#define MHZ19_TOPIC "weatherstation/co2"


// -- SGP30 Air Quality Sensor --
#define SGP30_I2C_ADDRESS 0x58
#define SGP30_TOPIC "weatherstation/airquality"

// -- PMS5003 Air Quality Sensor --
#define PMS5003_RX_PIN 18
#define PMS5003_TX_PIN 19
#define PMS5003_TOPIC "weatherstation/particles"

// -- Soil Moisture & Temp Sensor --
#define SOIL_MOISTURE_PIN 36 // ADC1_CH0
#define SOIL_TEMP_PIN 25     // 1-Wire data pin
#define SOIL_TOPIC "weatherstation/garden/soil"

// -- Geiger Counter (RadiationD-v1.1) --
#define GEIGER_COUNTER_PIN 39 // GPIO pin to connect to the board's pulse output
#define GEIGER_COUNTER_TOPIC "weatherstation/radiation"
#define GEIGER_CPM_TO_USV_H 0.0057 // Conversion factor for SBM-20 tube


// -- TIME CONFIGURATION --
#define NTP_SERVER "pool.ntp.org"
#define UTC_OFFSET_SECONDS 0
#define DAYLIGHT_OFFSET_SECONDS 3600


// -- UPDATE INTERVAL --
#define UPDATE_INTERVAL_MS 60000 // 60 seconds


// -- DEBUG & CONTROL --
#define DEBUG_CONTROL_TOPIC "weatherstation/control/debug"
extern bool g_debug_mode;


// -- OTA --
#define OTA_HOSTNAME "esp32-weatherstation"

#endif // CONFIG_H
