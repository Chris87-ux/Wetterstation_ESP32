#ifndef PMS5003_SENSOR_H
#define PMS5003_SENSOR_H

#include "Sensor.h"
#include <Adafruit_PM25AQI.h>

// This sensor provides multiple values.
// This class will publish them as a single JSON string.
class PMS5003Sensor : public Sensor {
public:
    PMS5003Sensor(HardwareSerial* serial, const String& topic);

    void setup() override;
    void read() override;
    String getValue() override;
    String getUnit() override;
    String getName() override;
    String getTopic() override;

private:
    Adafruit_PM25AQI _aqi = Adafruit_PM25AQI();
    HardwareSerial* _serial;
    String _topic;
    PM25_AQI_Data _data;
};

#endif // PMS5003_SENSOR_H
