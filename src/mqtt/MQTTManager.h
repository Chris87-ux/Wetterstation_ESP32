#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <WiFi.h>
#include <PubSubClient.h>
#include "../sensors/Sensor.h"
#include "../calculations/Calculation.h"
#include "../calculations/RainCalculation.h"
#include "../calculations/RadiationCalculation.h"
#include <vector>

class MQTTManager {
public:
    MQTTManager(const char* broker_ip, int broker_port, const char* client_id);

    void setup(const char* ssid, const char* password);
    void loop();
    void publishRain(RainCalculation* rainCalc);
    void publishRadiation(RadiationCalculation* radiationCalc);
    void publishData(const String& topic, const String& payload);
    void publishDebug(const String& topic, const String& payload);
    void publishHealthStatus(Sensor* sensor);

private:
    void connect();

    const char* _broker_ip;
    int _broker_port;
    const char* _client_id;

    WiFiClient _wifiClient;
    PubSubClient _mqttClient;
};

#endif // MQTT_MANAGER_H
