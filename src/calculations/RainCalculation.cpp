#include "RainCalculation.h"
#include "../sensors/PulseCounterSensor.h"

RainCalculation::RainCalculation(float mmPerPulse, TimeManager* timeManager)
    : _mmPerPulse(mmPerPulse), _timeManager(timeManager) {}

void RainCalculation::calculate(const std::vector<Sensor*>& sensors) {
    if (!_timeManager || !_timeManager->timeIsSynced()) {
        // We cannot perform time-based calculations without a synced clock
        return;
    }

    // First, check if the day, hour, etc. has rolled over
    checkForTimeRollover();

    // Find the rain gauge sensor
    for (const auto& sensor : sensors) {
        if (sensor->getName() == "Rain Gauge") {
            // Downcast to access the specific subclass method
            PulseCounterSensor* rainSensor = static_cast<PulseCounterSensor*>(sensor);
            if (rainSensor) {
                unsigned long newPulses = rainSensor->getAndResetPulseCount();
                if (newPulses > 0) {
                    float newRain = newPulses * _mmPerPulse;
                    _rainLastHour += newRain;
                    _rainToday += newRain;
                    _rainThisWeek += newRain;
                    _rainThisMonth += newRain;
                }
            }
            break; // Found the sensor, no need to continue looping
        }
    }
}

void RainCalculation::checkForTimeRollover() {
    int currentHour = _timeManager->getHour();
    int currentDay = _timeManager->getDay();
    int currentWday = _timeManager->getDayOfWeek(); // Day of week
    int currentMonth = _timeManager->getMonth();

    // Initialize trackers on first run
    if (_lastCalcHour == -1) {
        _lastCalcHour = currentHour;
        _lastCalcDay = currentDay;
        _lastCalcWday = currentWday;
        _lastCalcMonth = currentMonth;
        return;
    }

    // Check for hour rollover
    if (currentHour != _lastCalcHour) {
        _rainLastHour = 0; // Reset hourly counter
        _lastCalcHour = currentHour;
    }

    // Check for day rollover
    if (currentDay != _lastCalcDay) {
        _rainYesterday = _rainToday; // Today's rain becomes yesterday's
        _rainToday = 0;
        _lastCalcDay = currentDay;
    }

    // Check for week rollover (e.g., on Sunday)
    // This simple logic assumes the week starts on Sunday (0)
    if (currentWday < _lastCalcWday) {
        _rainThisWeek = 0;
    }
    _lastCalcWday = currentWday;


    // Check for month rollover
    if (currentMonth != _lastCalcMonth) {
        _rainThisMonth = 0;
        _lastCalcMonth = currentMonth;
    }
}
