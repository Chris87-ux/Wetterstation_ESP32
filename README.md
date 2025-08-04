# Modular ESP32 Weather Station

This project provides a modular and extensible firmware structure for building a weather station using an ESP32 microcontroller. It's designed to be easily configurable and allows for the simple addition of new sensors, calculations, and communication protocols.

The core features include:
-   **Modular Sensor Integration:** A plugin-style architecture for adding any type of sensor (Analog, Digital, I2C, SPI, etc.).
-   **Calculation Layer:** A system for deriving new values (e.g., Heat Index) from raw sensor data.
-   **MQTT Communication:** A robust MQTT client for publishing data to a broker.
-   **Easy Configuration:** A central `config.h` file for all your settings.

This project is designed to be built with [PlatformIO](https://platformio.org/).

## Project Structure

The project follows the standard PlatformIO directory structure:

```
.
├── include/              # For project-wide header files
├── lib/                  # For external libraries (managed by PlatformIO)
├── platformio.ini        # PlatformIO project configuration file
├── src/                  # Main source code
│   ├── calculations/     # Calculation logic
│   │   ├── Calculation.h # Base class for all calculations
│   │   └── HeatIndex.cpp # Example calculation
│   │   └── HeatIndex.h
│   ├── mqtt/             # MQTT communication logic
│   │   ├── MQTTManager.cpp
│   │   └── MQTTManager.h
│   ├── sensors/          # Sensor drivers and interfaces
│   │   ├── Sensor.h      # Base class for all sensors
│   │   ├── AnalogSensor.cpp
│   │   ├── AnalogSensor.h
│   │   ├── I2CSensor.cpp # Base class for I2C sensors
│   │   └── I2CSensor.h
│   ├── config.h          # Central configuration file
│   └── main.cpp          # Main application logic
└── test/                 # For unit tests
```

## Configuration

All project-specific settings can be configured in the `src/config.h` file. Before uploading, you must set:

1.  **Wi-Fi Credentials:**
    -   `WIFI_SSID`: Your Wi-Fi network name.
    -   `WIFI_PASSWORD`: Your Wi-Fi password.
2.  **MQTT Broker Details:**
    -   `MQTT_BROKER_IP`: The IP address or hostname of your MQTT broker.
    -   `MQTT_BROKER_PORT`: The port of your MQTT broker (usually 1883).
3.  **Sensor Pins and Addresses:**
    -   Update the pin numbers and I2C addresses for your specific sensors.

## How to Add a New Sensor

The modular design makes it simple to add new sensors. Here’s how you would add a new I2C sensor, for example, a BME280.

**1. Create the Sensor Class:**
Create new files `src/sensors/BME280Sensor.h` and `src/sensors/BME280Sensor.cpp`. You will likely need to add a library for the BME280 to `platformio.ini`, e.g., `adafruit/Adafruit BME280 Library`.

*__src/sensors/BME280Sensor.h__*
```cpp
#ifndef BME280_SENSOR_H
#define BME280_SENSOR_H

#include "I2CSensor.h"
#include <Adafruit_BME280.h>

// This class will actually represent multiple values (temp, humidity, pressure)
// For simplicity, this example just shows one. A better way would be to have one BME280
// object that provides multiple "sub-sensors". This is a good extension point.

class BME280_Temperature : public I2CSensor {
public:
    BME280_Temperature(uint8_t address, const String& topic);
    void setup() override;
    void read() override;
    String getValue() override;
    String getUnit() override;
private:
    Adafruit_BME280 _bme;
    float _temperature;
};

#endif
```

**2. Implement the Sensor Logic:**
In the `.cpp` file, you would implement the logic to read from the sensor using its library.

**3. Add to `main.cpp`:**
Finally, include your new sensor's header in `src/main.cpp` and add an instance of it to the `sensors` vector in the `setup()` function.

```cpp
#include "sensors/BME280Sensor.h"

// ... in setup() ...
sensors.push_back(new BME280_Temperature(BME280_I2C_ADDRESS, "weatherstation/livingroom/temperature"));
```

That's it! The main loop will automatically handle reading, calculating, and publishing the data from your new sensor.

## Building and Uploading

1.  Install [Visual Studio Code](https://code.visualstudio.com/) and the [PlatformIO IDE extension](https://platformio.org/platformio-ide).
2.  Open the project folder in VS Code.
3.  Modify `src/config.h` with your details.
4.  Click the "Upload" button in the PlatformIO toolbar (usually a right-pointing arrow in the bottom status bar).
