#include "MQTTManager.h"
#include "../config.h" // For MQTT credentials if needed

MQTTManager::MQTTManager(const char* broker_ip, int broker_port, const char* client_id)
    : _broker_ip(broker_ip), _broker_port(broker_port), _client_id(client_id), _mqttClient(_wifiClient) {}

void MQTTManager::setup(const char* ssid, const char* password) {
    Serial.print("Connecting to Wi-Fi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWi-Fi connected!");

    _mqttClient.setServer(_broker_ip, _broker_port);
}

void MQTTManager::loop() {
    if (! _mqttClient.connected()) {
        connect();
    }
    _mqttClient.loop();
}

void MQTTManager::connect() {
    while (! _mqttClient.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (_mqttClient.connect(_client_id, MQTT_USER, MQTT_PASSWORD)) {
            Serial.println("connected");
            // Here you can subscribe to topics if needed
            // _mqttClient.subscribe("some/topic");
        } else {
            Serial.print("failed, rc=");
            Serial.print(_mqttClient.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void MQTTManager::publishData(const String& topic, const String& payload) {
    Serial.print("Publishing to ");
    Serial.print(topic);
    Serial.print(": ");
    Serial.println(payload);
    _mqttClient.publish(topic.c_str(), payload.c_str());
}

void MQTTManager::publish(const std::vector<Sensor*>& sensors, const std::vector<Calculation*>& calculations) {
    if (!_mqttClient.connected()) {
        Serial.println("Cannot publish, MQTT not connected.");
        return;
    }

    // Publish sensor data
    for (const auto& sensor : sensors) {
        publishData(sensor->getTopic(), sensor->getValue());
    }

    // Publish calculation data
    for (const auto& calc : calculations) {
        publishData(calc->getTopic(), calc->getValue());
    }

    /*
    // Alternative approach: Publish all data as a single JSON object
    String json = "{";
    for (const auto& sensor : sensors) {
        json += "\"" + sensor->getName() + "\":\"" + sensor->getValue() + "\",";
    }
    for (const auto& calc : calculations) {
        json += "\"" + calc->getName() + "\":\"" + calc->getValue() + "\",";
    }
    // Remove last comma
    if (json.length() > 1) {
        json.remove(json.length() - 1);
    }
    json += "}";
    publishData("weatherstation/data", json);
    */
}
