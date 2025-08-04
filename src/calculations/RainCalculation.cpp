#include "RainCalculation.h"
#include "../sensors/PulseCounterSensor.h"

RainCalculation::RainCalculation(float mmPerPulse, TimeManager* timeManager)
    : _mmPerPulse(mmPerPulse), _timeManager(timeManager) {}

void RainCalculation::calculate(const std::vector<Sensor*>& sensors) {
    if (!_timeManager || !_timeManager->timeIsSynced()) {
        return;
    }

    // Rollover logic must be checked *before* adding new rain
    checkForTimeRollover();

    for (const auto& sensor : sensors) {
        if (sensor->getName() == "Rain Gauge") {
            PulseCounterSensor* rainSensor = static_cast<PulseCounterSensor*>(sensor);
            if (rainSensor) {
                unsigned long newPulses = rainSensor->getAndResetPulseCount();
                if (newPulses > 0) {
                    float newRain = newPulses * _mmPerPulse;
                    // Add to the current minute, day, week, month
                    _rainPast60Mins[59] += newRain;
                    _rainToday += newRain;
                    _rainThisWeek += newRain;
                    _rainThisMonth += newRain;
                }
            }
            break;
        }
    }
}

float RainCalculation::getRainLastHour() const {
    float total = 0;
    for (int i = 0; i < 60; i++) {
        total += _rainPast60Mins[i];
    }
    return total;
}

void RainCalculation::checkForTimeRollover() {
    int currentMinute = _timeManager->getMinute();
    int currentDay = _timeManager->getDay();
    int currentWday = _timeManager->getDayOfWeek();
    int currentMonth = _timeManager->getMonth();

    if (_lastCalcMinute == -1) { // First run initialization
        _lastCalcMinute = currentMinute;
        _lastCalcDay = currentDay;
        _lastCalcWday = currentWday;
        _lastCalcMonth = currentMonth;
        return;
    }

    // Check for minute rollover for the sliding window
    if (currentMinute != _lastCalcMinute) {
        // Shift the array
        for (int i = 0; i < 59; i++) {
            _rainPast60Mins[i] = _rainPast60Mins[i+1];
        }
        _rainPast60Mins[59] = 0; // Clear the current minute's slot
        _lastCalcMinute = currentMinute;
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
