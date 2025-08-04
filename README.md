# Modular ESP32 Weather Station

This project provides a modular and extensible firmware structure for building a weather station using an ESP32 microcontroller. It's designed to be easily configurable and allows for the simple addition of new sensors, calculations, and communication protocols.

The core features include:
-   **Modular Sensor Integration:** A plugin-style architecture for adding any type of sensor.
-   **Advanced Calculation Layer:** A system for deriving new values from raw sensor data, including time-based calculations for rainfall.
-   **NTP Time Sync:** Automatically synchronizes time to ensure accurate calculations.
-   **MQTT Communication:** A robust MQTT client for publishing data to a broker.
-   **Centralized Configuration:** A single `config.h` file for all your settings.

This project is designed to be built with [PlatformIO](https://platformio.org/).

## Advanced Calculations

### Rain Measurement
The system now performs advanced, time-based calculations for rainfall. In addition to the raw pulse count from the sensor, it calculates and publishes the following metrics to dedicated MQTT topics:
-   Rainfall in the last hour
-   Total rainfall for today
-   Total rainfall for yesterday
-   Total rainfall for the current week
-   Total rainfall for the current month

The unit for all totals is millimeters (mm), which is equivalent to liters per square meter (l/m²).

## Sensors Implemented

This project includes drivers and examples for the following sensors:

| Measurement       | Sensor            | Interface |
| ----------------- | ----------------- | --------- |
| Gas               | MQ-2              | Analog    |
| UV                | GY-8511           | Analog    |
| Wind Direction    | WH-SP-WD          | Analog    |
| Rain              | MS-WH-SP-RG       | Pulse     |
| Wind Speed        | WH-SP-WS01        | Pulse     |
| Light             | GY-302 (BH1750)   | I2C       |
| Lightning         | AS3935            | I2C       |
| CO₂               | MH-Z19E           | UART      |

## Configuration

All project-specific settings are centralized in the `src/config.h` file. Before uploading, you must review and set the following:

1.  **Wi-Fi Credentials:** `WIFI_SSID` and `WIFI_PASSWORD`.
2.  **MQTT Broker Details:** `MQTT_BROKER_IP` and `MQTT_BROKER_PORT`.
3.  **Time Configuration:**
    *   `NTP_SERVER`: The NTP server for time synchronization.
    *   `UTC_OFFSET_SECONDS`: Your time zone's offset from UTC in seconds.
    *   `DAYLIGHT_OFFSET_SECONDS`: Your daylight saving offset in seconds (usually 3600 or 0).
4.  **Sensor-Specific Configuration:**
    *   Review all pin definitions, I2C addresses, and MQTT topics for each sensor.
    *   **Crucially, set the `RAIN_MM_PER_PULSE`** to match your specific rain gauge's specification (e.g., 0.2794 mm per tip).
    *   Adjust `WIND_KMH_PER_PULSE_PER_SEC` for your anemometer.

### Pinout Configuration

The default pin assignments are defined in `src/config.h`. The standard I2C pins for the ESP32 are GPIO 22 (SCL) and GPIO 21 (SDA).

| Sensor            | Pin                | ESP32 GPIO |
| ----------------- | ------------------ | ---------- |
| MQ-2 Gas          | Analog In          | 32         |
| GY-8511 UV        | Analog In          | 33         |
| Wind Direction    | Analog In          | 35         |
| Rain Gauge        | Pulse Input        | 26         |
| Wind Speed        | Pulse Input        | 27         |
| AS3935 Lightning  | IRQ                | 4          |
| MH-Z19E CO₂       | RX (Sensor TX)     | 16         |
| MH-Z19E CO₂       | TX (Sensor RX)     | 17         |

## Building and Uploading

1.  Install [Visual Studio Code](https://code.visualstudio.com/) and the [PlatformIO IDE extension](https://platformio.org/platformio-ide).
2.  Open the project folder in VS Code.
3.  Modify `src/config.h` with your details.
4.  Click the "Upload" button in the PlatformIO toolbar.
