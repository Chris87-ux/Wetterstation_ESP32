#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <WiFi.h>
#include <PubSubClient.h>
#include "../sensors/Sensor.h"
#include "../calculations/Calculation.h"
#include <vector>

class MQTTManager {
public:
    MQTTManager(const char* broker_ip, int broker_port, const char* client_id);

    void setup(const char* ssid, const char* password);
    void loop();
    void publish(const std::vector<Sensor*>& sensors, const std::vector<Calculation*>& calculations);

private:
    void connect();
    void publishData(const String& topic, const String& payload);

    const char* _broker_ip;
    int _broker_port;
    const char* _client_id;

    WiFiClient _wifiClient;
    PubSubClient _mqttClient;
};

#endif // MQTT_MANAGER_H
