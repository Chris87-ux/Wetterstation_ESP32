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

// Example for a digital sensor
#define DIGITAL_SENSOR_PIN 25

// Example for an I2C sensor
#define BME280_I2C_ADDRESS 0x76


// -- UPDATE INTERVAL --
#define UPDATE_INTERVAL_MS 60000 // 60 seconds

#endif // CONFIG_H
