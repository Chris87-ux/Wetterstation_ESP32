#include "MQTTManager.h"
#include "../config.h" // For MQTT credentials if needed

// MQTT callback function
void mqtt_callback(char* topic, byte* payload, unsigned int length) {
    String message;
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    message.toLowerCase();

    if (String(topic) == DEBUG_CONTROL_TOPIC) {
        if (message == "on" || message == "true" || message == "1") {
            g_debug_mode = true;
            Serial.println("MQTT Debug Mode: ON");
        } else {
            g_debug_mode = false;
            Serial.println("MQTT Debug Mode: OFF");
        }
    }
}

MQTTManager::MQTTManager(const char* broker_ip, int broker_port, const char* client_id)
    : _broker_ip(broker_ip), _broker_port(broker_port), _client_id(client_id), _mqttClient(_wifiClient) {}

void MQTTManager::setup(const char* ssid, const char* password) {
    _mqttClient.setCallback(mqtt_callback);
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
            // Subscribe to the debug control topic
            _mqttClient.subscribe(DEBUG_CONTROL_TOPIC);
            Serial.print("Subscribed to: ");
            Serial.println(DEBUG_CONTROL_TOPIC);
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

void MQTTManager::publishRain(RainCalculation* rainCalc) {
    if (!rainCalc) return;

    String baseTopic = rainCalc->getTopic();
    publishData(baseTopic + "/last_hour", String(rainCalc->getRainLastHour()));
    publishData(baseTopic + "/today", String(rainCalc->getRainToday()));
    publishData(baseTopic + "/yesterday", String(rainCalc->getRainYesterday()));
    publishData(baseTopic + "/this_week", String(rainCalc->getRainThisWeek()));
    publishData(baseTopic + "/this_month", String(rainCalc->getRainThisMonth()));
}

void MQTTManager::publishDebug(const String& topic, const String& payload) {
    if (g_debug_mode) {
        publishData(topic, payload);
    }
}

void MQTTManager::publishHealthStatus(Sensor* sensor) {
    if (!sensor) return;
    String healthTopic = "weatherstation/health/" + sensor->getName();
    healthTopic.replace(" ", "_"); // Sanitize topic name
    String payload = sensor->isHealthy() ? "OK" : "FAIL";
    publishData(healthTopic, payload);
}

void MQTTManager::publishRadiation(RadiationCalculation* radiationCalc) {
    if (!radiationCalc) return;

    String baseTopic = radiationCalc->getTopic();
    publishData(baseTopic + "/dose", String(radiationCalc->getCurrentDoseRate()));
    publishData(baseTopic + "/cpm_avg_last_hour", String(radiationCalc->getAverageCPM_LastHour()));
    publishData(baseTopic + "/cpm_peak_today", String(radiationCalc->getPeakCPM_Today()));
    publishData(baseTopic + "/cpm_peak_yesterday", String(radiationCalc->getPeakCPM_Yesterday()));
}
