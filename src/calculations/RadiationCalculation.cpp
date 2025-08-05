#include "RadiationCalculation.h"
#include "../sensors/PulseCounterSensor.h" // Geiger counter is a pulse sensor

RadiationCalculation::RadiationCalculation(float uSvPerHourFactor, TimeManager* timeManager)
    : _uSvPerHourFactor(uSvPerHourFactor), _timeManager(timeManager) {}

void RadiationCalculation::calculate(const std::vector<Sensor*>& sensors) {
    if (!_timeManager || !_timeManager->timeIsSynced()) {
        return;
    }

    checkForTimeRollover();

    for (const auto& sensor : sensors) {
        if (sensor->getName() == "Geiger Counter") {
            // The sensor's value is already CPM thanks to the conversion factor in its constructor
            _currentCPM = sensor->getValue().toFloat();

            // Update dose rate
            _currentDoseRate = _currentCPM * _uSvPerHourFactor;

            // Update today's peak
            if (_currentCPM > _cpmTodayPeak) {
                _cpmTodayPeak = _currentCPM;
            }

            // Update running average for the last hour
            _cpmSum_LastHour += _currentCPM;
            _cpmSamples_LastHour++;
            _cpmLast60MinsAvg = _cpmSum_LastHour / _cpmSamples_LastHour;

            break;
        }
    }
}

void RadiationCalculation::checkForTimeRollover() {
    int currentHour = _timeManager->getHour();
    int currentDay = _timeManager->getDay();

    if (_lastCalcDay == -1) { // First run
        _lastCalcDay = currentDay;
        _lastCalcHour = currentHour;
        return;
    }

    if (currentHour != _lastCalcHour) {
        // Reset hourly average
        _cpmSum_LastHour = 0;
        _cpmSamples_LastHour = 0;
        _cpmLast60MinsAvg = 0;
        _lastCalcHour = currentHour;
    }

    if (currentDay != _lastCalcDay) {
        // Shift peak values
        _cpmYesterdayPeak = _cpmTodayPeak;
        _cpmTodayPeak = 0;
        _lastCalcDay = currentDay;
    }
}
