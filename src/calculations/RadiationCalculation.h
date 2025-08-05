#ifndef RADIATION_CALCULATION_H
#define RADIATION_CALCULATION_H

#include "Calculation.h"
#include "../time/TimeManager.h"

// This class calculates radiation statistics over various time periods.
class RadiationCalculation : public Calculation {
public:
    RadiationCalculation(float uSvPerHourFactor, TimeManager* timeManager);

    void calculate(const std::vector<Sensor*>& sensors) override;

    // Base class methods
    String getName() override { return "Radiation"; }
    String getValue() override { return String(getCurrentDoseRate()); }
    String getUnit() override { return "uSv/h"; }
    String getTopic() override { return "weatherstation/radiation"; }

    // New getters for specific values
    float getAverageCPM_LastHour() const { return _cpmLast60MinsAvg; }
    float getPeakCPM_Today() const { return _cpmTodayPeak; }
    float getPeakCPM_Yesterday() const { return _cpmYesterdayPeak; }
    float getCurrentDoseRate() const { return _currentDoseRate; }

private:
    void checkForTimeRollover();

    TimeManager* _timeManager;
    float _uSvPerHourFactor; // Factor to convert CPM to uSv/h

    // Storage for radiation values
    float _currentCPM = 0.0;
    float _currentDoseRate = 0.0;
    float _cpmLast60MinsAvg = 0.0;
    float _cpmTodayPeak = 0.0;
    float _cpmYesterdayPeak = 0.0;

    // For averaging
    float _cpmSum_LastHour = 0.0;
    int _cpmSamples_LastHour = 0;

    // State to track time
    int _lastCalcDay = -1;
    int _lastCalcHour = -1;
};

#endif // RADIATION_CALCULATION_H
