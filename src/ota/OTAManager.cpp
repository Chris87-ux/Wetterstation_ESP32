#include "OTAManager.h"
#include <ArduinoOTA.h>
#include <WiFi.h>

OTAManager::OTAManager(const char* hostname) : _hostname(hostname) {}

void OTAManager::setup() {
    // Only start OTA if we are connected to Wi-Fi
    if (WiFi.status() != WL_CONNECTED) {
        return;
    }

    ArduinoOTA.setHostname(_hostname);

    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
            type = "sketch";
        } else { // U_SPIFFS
            type = "filesystem";
        }
        Serial.println("Start updating " + type);
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

    ArduinoOTA.begin();
    Serial.print("OTA Initialized. Ready for updates on hostname: ");
    Serial.println(_hostname);
}

void OTAManager::loop() {
    ArduinoOTA.handle();
}
