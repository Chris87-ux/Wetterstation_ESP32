#ifndef MHZ19_CO2_SENSOR_H
#define MHZ19_CO2_SENSOR_H

#include "Sensor.h"
#include <MHZ19.h>

class MHZ19_CO2Sensor : public Sensor {
public:
    // The constructor takes a pointer to the HardwareSerial port (e.g., &Serial2)
    MHZ19_CO2Sensor(HardwareSerial* serial, const String& topic);

    void setup() override;
    void read() override;
    String getValue() override;
    String getUnit() override;
    String getName() override;
    String getTopic() override;

private:
    MHZ19 _co2_sensor;
    String _topic;
    int _co2_ppm = 0;
};

#endif // MHZ19_CO2_SENSOR_H
