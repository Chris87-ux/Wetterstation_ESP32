# Modular ESP32 Weather Station

This project provides a modular and extensible firmware structure for building a weather station using an ESP32 microcontroller. It's designed to be easily configurable and allows for the simple addition of new sensors, calculations, and communication protocols.

The core features include:
-   **Modular Sensor Integration:** A plugin-style architecture for adding any type of sensor (Analog, Digital, I2C, SPI, etc.).
-   **Calculation Layer:** A system for deriving new values (e.g., Heat Index) from raw sensor data.
-   **MQTT Communication:** A robust MQTT client for publishing data to a broker.
-   **Easy Configuration:** A central `config.h` file for all your settings.

This project is designed to be built with [PlatformIO](https://platformio.org/).

## Sensors Implemented

This project now includes drivers and examples for the following sensors:

| Measurement       | Sensor            | Interface | Driver Class            | Notes                                   |
| ----------------- | ----------------- | --------- | ----------------------- | --------------------------------------- |
| Gas               | MQ-2              | Analog    | `AnalogSensor`          | Reads raw analog value.                 |
| UV                | GY-8511           | Analog    | `AnalogSensor`          | Reads raw analog value.                 |
| Wind Direction    | WH-SP-WD          | Analog    | `WindDirectionSensor`   | Maps analog value to degrees.           |
| Rain              | MS-WH-SP-RG       | Pulse     | `PulseCounterSensor`    | Counts pulses via interrupt.            |
| Wind Speed        | WH-SP-WS01        | Pulse     | `PulseCounterSensor`    | Measures pulse frequency via interrupt. |
| Light             | GY-302 (BH1750)   | I2C       | `BH1750Sensor`          | Uses the `claws/BH1750` library.        |
| Lightning         | AS3935            | I2C       | `AS3935Sensor`          | Uses the `raivisr/AS3935` library.      |
| CO₂               | MH-Z19E           | UART      | `MHZ19_CO2Sensor`       | Uses the `WifWaf/MH-Z19` library.       |

## Pinout Configuration

The default pin assignments are defined in `src/config.h`. You can change these to match your hardware setup. The standard I2C pins for the ESP32 are GPIO 22 (SCL) and GPIO 21 (SDA).

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

## Project Structure

The project follows the standard PlatformIO directory structure. Key sensor implementations can be found in `src/sensors/`.

```
.
├── platformio.ini        # PlatformIO project configuration file
...
```

## How to Add a New Sensor

The modular design makes it simple to add new sensors. For a new sensor, you can typically copy one of the existing sensor classes and adapt it for the new hardware and library.

1.  **Add Library (if needed):** Add the required library to `platformio.ini`.
2.  **Create Sensor Class:** Create a new `.h` and `.cpp` file in `src/sensors/`.
3.  **Add to `config.h`:** Add any new pin or address definitions.
4.  **Add to `main.cpp`:** Add an instance of it to the `sensors` vector in `setup()`.

## Building and Uploading

1.  Install [Visual Studio Code](https://code.visualstudio.com/) and the [PlatformIO IDE extension](https://platformio.org/platformio-ide).
2.  Open the project folder in VS Code.
3.  Modify `src/config.h` with your details.
4.  Click the "Upload" button in the PlatformIO toolbar.
