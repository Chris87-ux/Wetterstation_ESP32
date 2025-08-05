# Modular ESP32 Weather Station (Professional Edition)

This project provides a professional-grade, modular, and extensible firmware structure for building a feature-rich weather station using an ESP32 microcontroller. It is designed for high reliability, easy configuration, and future expansion.

## Core Features
-   **Fully Modular:** Every sensor is conditionally compiled. Enable or disable features by changing a single line in `config.h`.
-   **Robust & Safe:** The code includes health checks for all sensors on startup and uses safe memory management practices.
-   **Advanced Calculations:** Includes time-based statistics for rainfall and radiation.
-   **Remote Management:**
    -   **Over-the-Air (OTA) Updates:** Update the firmware remotely over Wi-Fi.
    -   **MQTT Health Status:** Publishes the boot-up health status of each sensor.
    -   **MQTT Remote Debugging:** Toggle a debug mode via MQTT to receive detailed sensor data for troubleshooting.
-   **NTP Time Sync:** Automatically synchronizes time to ensure accurate timestamps and calculations.
-   **Centralized Configuration:** A single, clean `config.h` file for all your settings.

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
| **Ionizing Radiation**          | RadiationD-v1.1   | Pulse     | *Disabled*     |

## Special Sensor: Geiger Counter Integration

A unique feature of this project is the ability to integrate a DIY Geiger counter like the **RadiationD-v1.1 (CAJOE)** kit.

### Hardware Modification
To connect the board to your ESP32, you need to tap into its pulse output signal. This provides a clean digital pulse for every radiation particle detected by the Geiger tube.

1.  **Locate the Pulse Signal:** On the RadiationD-v1.1 PCB, find the components related to the audio output (the buzzer/beeper). There is typically a resistor leading to the base of the transistor that drives the buzzer. The point between this resistor and the transistor is an ideal place to get the signal.
2.  **Solder a Connection Wire:** Carefully solder a single wire to this point.
3.  **Connect to ESP32:** Connect this wire to the GPIO pin defined as `GEIGER_COUNTER_PIN` in `config.h` (default is GPIO 39). Also, ensure you have a common ground (`GND`) connection between the Geiger counter board and the ESP32.

### Configuration
-   Enable the sensor in `config.h` by uncommenting `#define USE_GEIGER_COUNTER`.
-   Set `GEIGER_COUNTER_PIN` to the GPIO pin you used.
-   **Crucially, set `GEIGER_CPM_TO_USV_H`**. This is the conversion factor for your specific Geiger tube (e.g., an SBM-20 tube has a factor of `0.0057`). This converts Counts Per Minute (CPM) into the standard dose rate of microsieverts per hour (µSv/h).

### MQTT Topics
When enabled, the following radiation topics will be published:
-   `.../radiation/dose`: Current estimated dose rate in µSv/h.
-   `.../radiation/cpm_avg_last_hour`: Average CPM over the last 60 minutes.
-   `.../radiation/cpm_peak_today`: Highest CPM value recorded today.
-   `.../radiation/cpm_peak_yesterday`: Highest CPM value recorded yesterday.

## General Configuration
All other project settings are in `src/config.h`. Remember to set your Wi-Fi, MQTT, and Time Zone details, and review all sensor pin assignments and calibration factors.
