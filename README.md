# Modular ESP32 Weather Station (Professional Edition)

This project provides a professional-grade, modular, and extensible firmware structure for building a feature-rich weather station using an ESP32 microcontroller. It is designed for high reliability, easy configuration, and future expansion.

## Core Features
-   **Fully Modular:** Every sensor is conditionally compiled, allowing you to enable or disable features by changing a single line in the configuration file.
-   **Robust & Safe:** The code includes health checks for all sensors on startup and uses safe memory management practices (no dynamic `new` or `delete`).
-   **Advanced Calculations:** Includes time-based calculations for rainfall (last hour, today, yesterday, etc.) using a true 60-minute sliding window.
-   **NTP Time Sync:** Automatically synchronizes time with an NTP server to ensure accurate timestamps and calculations.
-   **MQTT Communication:** Publishes all sensor and calculation data to a structured MQTT topic hierarchy.
-   **Centralized Configuration:** A single, clean `config.h` file for all your settings.

## Sensor Suite

This project contains drivers for a wide array of environmental sensors.

| Measurement                     | Sensor            | Interface | Status    |
| ------------------------------- | ----------------- | --------- | --------- |
| **Ambient Light**               | LDR               | Analog    | Enabled   |
| **Gas (Propane, Methane etc.)** | MQ-2              | Analog    | Enabled   |
| **UV Index**                    | GY-8511           | Analog    | Enabled   |
| **Wind Direction**              | WH-SP-WD          | Analog    | Enabled   |
| **Rainfall**                    | MS-WH-SP-RG       | Pulse     | Enabled   |
| **Wind Speed**                  | WH-SP-WS01        | Pulse     | Enabled   |
| **Light (Lux)**                 | GY-302 (BH1750)   | I2C       | Enabled   |
| **Lightning Detection**         | AS3935            | I2C       | Enabled   |
| **CO₂**                         | MH-Z19E           | UART      | Enabled   |
| **Air Quality (Gases)**         | SGP30             | I2C       | *Disabled*|
| **Air Quality (Particulates)**  | Plantower PMS5003 | UART      | *Disabled*|
| **Soil Moisture & Temp**        | Capacitive + DS18B20 | Analog/1-Wire | *Disabled*|

## Configuration

All project configuration is done in `src/config.h`.

### 1. Enabling and Disabling Sensors
To enable or disable a sensor, simply find its corresponding `#define` in the `--- Enable/Disable Sensors ---` section of `config.h` and comment or uncomment it. For example, to disable the LDR sensor:
```cpp
// #define USE_LDR
```
The new advanced sensors are disabled by default.

### 2. Sensor-Specific Settings
Below the enable/disable section, you can find the detailed configuration for each sensor, including GPIO pins, I2C addresses, MQTT topics, and crucial conversion factors.

**Most Important Settings:**
-   **`RAIN_MM_PER_PULSE`**: You **must** calibrate this to match your specific rain gauge. It is the amount of rain (in mm) for each tip of the gauge's bucket.
-   **`WIND_KMH_PER_PULSE_PER_SEC`**: Calibrate this based on your anemometer's documentation.
-   **Pin Assignments**: Ensure all `_PIN` and `_ADDRESS` values match your physical wiring.

### 3. Network and Time Configuration
-   Set your `WIFI_SSID` and `WIFI_PASSWORD`.
-   Set your `MQTT_BROKER_IP`.
-   Set your time zone offset from UTC (`UTC_OFFSET_SECONDS`) and your daylight saving offset (`DAYLIGHT_OFFSET_SECONDS`).

## Future Expansion: Suggested Calculations

With this rich sensor data, you can implement many more calculations in the `src/calculations` directory. Here are some ideas:

| Calculation          | Requires Sensors                | Description                                                                 |
| -------------------- | ------------------------------- | --------------------------------------------------------------------------- |
| **Dew Point**        | Temperature, Humidity           | A better measure of how humid it feels than relative humidity alone.        |
| **Wind Chill**       | Temperature, Wind Speed         | How cold it feels when wind is a factor (at low temperatures).              |
| **AQI (Air Quality Index)** | Particulate (PMS) or Gas (SGP30) | A standardized index (0-500) for reporting air pollution levels.          |
| **Cloud Base (Est.)**  | Temperature, Dew Point          | An estimation of the altitude of the cloud base.                            |
| **Wind Rose Data**   | Wind Speed, Wind Direction      | A statistical summary of wind speed and direction over time.                |

## Building and Uploading
1.  Install [Visual Studio Code](https://code.visualstudio.com/) and the [PlatformIO IDE extension](https://platformio.org/platformio-ide).
2.  Open the project folder in VS Code.
3.  Modify `src/config.h` with your details.
4.  Click the "Upload" button in the PlatformIO toolbar.
