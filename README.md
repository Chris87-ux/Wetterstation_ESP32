# Modular ESP32 Weather Station (Professional Edition)

This project provides a professional-grade, modular, and extensible firmware structure for building a feature-rich weather station using an ESP32 microcontroller. It is designed for high reliability, easy configuration, and future expansion.

## Core Features
-   **Fully Modular:** Every sensor is conditionally compiled. Enable or disable features by changing a single line in `config.h`.
-   **Robust & Safe:** The code includes health checks for all sensors on startup and uses safe memory management practices.
-   **Advanced Calculations:** Includes time-based calculations for rainfall (60-minute sliding window, daily, weekly, monthly).
-   **Remote Management:**
    -   **Over-the-Air (OTA) Updates:** Update the firmware remotely over Wi-Fi.
    -   **MQTT Health Status:** Publishes the boot-up health status of each sensor.
    -   **MQTT Remote Debugging:** Toggle a debug mode via MQTT to receive detailed sensor data for troubleshooting.
-   **NTP Time Sync:** Automatically synchronizes time to ensure accurate calculations.
-   **Centralized Configuration:** A single, clean `config.h` file for all your settings.

## Remote Management

### Over-the-Air (OTA) Updates
Once the device is on your Wi-Fi network, you can upload new firmware without a physical connection. In PlatformIO, this is typically done by setting an `upload_port` with the device's IP address or hostname in `platformio.ini` and using the "Upload" command. The default hostname is configured in `config.h`.

### MQTT Health & Debugging
The station uses a base topic (`weatherstation/`) which can be configured.
-   **Health Status:** On boot, the status of each enabled sensor is published to `weatherstation/health/<Sensor_Name>` (e.g., `weatherstation/health/Wind_Direction`). The payload will be `OK` or `FAIL`.
-   **Remote Debugging:**
    1.  Publish a message (`on`, `true`, or `1`) to the topic `weatherstation/control/debug` to enable debug mode.
    2.  Publish any other message (`off`, `false`, `0`) to the same topic to disable it.
    3.  When enabled, each sensor will publish detailed raw data or status messages to its own debug topic (e.g., `weatherstation/livingroom/winddir/debug`).

## Sensor Suite

This project contains drivers for a wide array of environmental sensors.

| Measurement                     | Sensor            | Interface | Default Status |
| ------------------------------- | ----------------- | --------- | -------------- |
| **Ambient Light**               | LDR               | Analog    | Enabled        |
| **Gas (Propane, Methane etc.)** | MQ-2              | Analog    | Enabled        |
| **UV Index**                    | GY-8511           | Analog    | Enabled        |
| **Wind Direction**              | WH-SP-WD          | Analog    | Enabled        |
| **Rainfall**                    | MS-WH-SP-RG       | Pulse     | Enabled        |
| **Wind Speed**                  | WH-SP-WS01        | Pulse     | Enabled        |
| **Light (Lux)**                 | GY-302 (BH1750)   | I2C       | Enabled        |
| **Lightning Detection**         | AS3935            | I2C       | Enabled        |
| **CO₂**                         | MH-Z19E           | UART      | Enabled        |
| **Air Quality (Gases)**         | SGP30             | I2C       | *Disabled*     |
| **Air Quality (Particulates)**  | Plantower PMS5003 | UART      | *Disabled*     |
| **Soil Moisture & Temp**        | Capacitive + DS18B20 | Analog/1-Wire | *Disabled*|

## Configuration

All project configuration is done in `src/config.h`.

1.  **Enable/Disable Sensors:** Find the `--- Enable/Disable Sensors ---` section and comment or uncomment the `#define` for each sensor you want to use.
2.  **Set Network Credentials:** Configure your `WIFI_SSID`, `WIFI_PASSWORD`, and `MQTT_BROKER_IP`.
3.  **Set Time Zone:** Configure `UTC_OFFSET_SECONDS` and `DAYLIGHT_OFFSET_SECONDS`.
4.  **Calibrate & Configure Sensors:** Review all pin assignments, I2C addresses, and especially the `RAIN_MM_PER_PULSE` and `WIND_KMH_PER_PULSE_PER_SEC` constants to match your hardware.

## Future Expansion: Suggested Calculations

With this rich sensor data, you can implement many more calculations in the `src/calculations` directory.

| Calculation          | Requires Sensors                | Description                                         |
| -------------------- | ------------------------------- | --------------------------------------------------- |
| **Dew Point**        | Temperature, Humidity           | A better measure of how "humid" it feels.           |
| **Wind Chill**       | Temperature, Wind Speed         | How cold it feels when wind is a factor.            |
| **AQI (Air Quality Index)** | Particulate (PMS) or Gas (SGP30) | A standardized index for reporting air pollution. |
| **Cloud Base (Est.)**  | Temperature, Dew Point          | An estimation of the altitude of the cloud base.    |
