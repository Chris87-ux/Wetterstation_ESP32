#ifndef OTA_MANAGER_H
#define OTA_MANAGER_H

#include <Arduino.h>

class OTAManager {
public:
    OTAManager(const char* hostname);
    void setup();
    void loop();

private:
    const char* _hostname;
};

#endif // OTA_MANAGER_H
