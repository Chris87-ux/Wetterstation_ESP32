#include "TimeManager.h"

TimeManager::TimeManager(const char* ntpServer, long utcOffset, int daylightOffset)
    : _ntpServer(ntpServer), _utcOffset(utcOffset), _daylightOffset(daylightOffset) {}

void TimeManager::setup() {
    // Init and get the time
    configTime(_utcOffset, _daylightOffset, _ntpServer);
    if (!getLocalTime(&_timeinfo)) {
        Serial.println("Failed to obtain time");
        return;
    }
    Serial.println("Time synchronized via NTP.");
    Serial.println(getFormattedTime().c_str());
}

bool TimeManager::timeIsSynced() {
    return getLocalTime(&_timeinfo);
}

String TimeManager::getFormattedTime() {
    if (timeIsSynced()) {
        char buffer[20];
        strftime(buffer, sizeof(buffer), "%A, %B %d %Y %H:%M:%S", &_timeinfo);
        return String(buffer);
    }
    return "Time not synced";
}

int TimeManager::getHour() {
    if (timeIsSynced()) return _timeinfo.tm_hour;
    return -1;
}

int TimeManager::getMinute() {
    if (timeIsSynced()) return _timeinfo.tm_min;
    return -1;
}

int TimeManager::getDay() {
    if (timeIsSynced()) return _timeinfo.tm_mday;
    return -1;
}

int TimeManager::getMonth() {
    if (timeIsSynced()) return _timeinfo.tm_mon + 1; // tm_mon is 0-11
    return -1;
}

int TimeManager::getYear() {
    if (timeIsSynced()) return _timeinfo.tm_year + 1900; // tm_year is years since 1900
    return -1;
}

int TimeManager::getDayOfWeek() {
    if (timeIsSynced()) return _timeinfo.tm_wday;
    return -1;
}
