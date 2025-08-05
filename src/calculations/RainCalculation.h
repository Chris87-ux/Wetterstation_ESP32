#ifndef RAIN_CALCULATION_H
#define RAIN_CALCULATION_H

#include "Calculation.h"
#include "../time/TimeManager.h"

// This class calculates rainfall over various time periods.
// It relies on an external TimeManager to know when days, hours, etc., change.
class RainCalculation : public Calculation {
public:
    RainCalculation(float mmPerPulse, TimeManager* timeManager);

    void calculate(const std::vector<Sensor*>& sensors) override;

    // These methods are not used for this multi-value calculation,
    // but must be implemented to satisfy the base class.
    String getName() override { return "Rain Totals"; }
    String getValue() override { return String(getRainToday()); }
    String getUnit() override { return "mm"; }
    String getTopic() override { return "weatherstation/rain"; } // Base topic

    // New getters for specific values
    float getRainLastHour() const;
    float getRainToday() const { return _rainToday; }
    float getRainYesterday() const { return _rainYesterday; }
    float getRainThisWeek() const { return _rainThisWeek; }
    float getRainThisMonth() const { return _rainThisMonth; }

private:
    void checkForTimeRollover();

    TimeManager* _timeManager;
    float _mmPerPulse;

    // Storage for rain values
    float _rainLastHour = 0.0;
    float _rainToday = 0.0;
    float _rainYesterday = 0.0;
    float _rainThisWeek = 0.0;
    float _rainThisMonth = 0.0;

    // State to track time
    int _lastCalcMinute = -1;
    int _lastCalcHour = -1;
    int _lastCalcDay = -1;
    int _lastCalcWday = -1;
    int _lastCalcMonth = -1;

    // For true 60-minute sliding window
    float _rainPast60Mins[60] = {0.0};
};

#endif // RAIN_CALCULATION_H
