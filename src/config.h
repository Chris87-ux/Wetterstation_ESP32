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

// -- SENSOR PINS & ADDRESSES --
// Example for an analog sensor
#define ANALOG_SENSOR_PIN 34

// -- New Analog Sensors --
#define MQ2_GAS_PIN 32
#define GY8511_UV_PIN 33
#define WIND_DIRECTION_PIN 35

// Example for a digital sensor
#define DIGITAL_SENSOR_PIN 25

// -- Pulse Counter Sensors --
#define RAIN_GAUGE_PIN 26
#define WIND_SPEED_PIN 27

// Example for an I2C sensor
#define BME280_I2C_ADDRESS 0x76

// -- New I2C Sensors --
#define BH1750_I2C_ADDRESS 0x23
#define AS3935_I2C_ADDRESS 0x03
#define AS3935_IRQ_PIN 4

// -- UART Sensor Pins --
#define MHZ19_RX_PIN 16
#define MHZ19_TX_PIN 17


// -- UPDATE INTERVAL --
#define UPDATE_INTERVAL_MS 60000 // 60 seconds

#endif // CONFIG_H
