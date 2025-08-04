#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

#include <Arduino.h>
#include "time.h"

class TimeManager {
public:
    TimeManager(const char* ntpServer, long utcOffset, int daylightOffset);
    void setup();
    bool timeIsSynced();
    String getFormattedTime();
    int getHour();
    int getDay();
    int getMonth();
    int getYear();
    int getDayOfWeek(); // 0=Sun, 1=Mon, ...

private:
    const char* _ntpServer;
    long _utcOffset;
    int _daylightOffset;
    struct tm _timeinfo;
};

#endif // TIME_MANAGER_H
