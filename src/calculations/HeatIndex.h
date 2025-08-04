#ifndef HEAT_INDEX_H
#define HEAT_INDEX_H

#include "Calculation.h"

class HeatIndex : public Calculation {
public:
    HeatIndex(const String& topic);

    void calculate(const std::vector<Sensor*>& sensors) override;

    String getName() override;
    String getValue() override;
    String getUnit() override;
    String getTopic() override;

private:
    String _topic;
    float _heatIndex = 0.0;

    // Function to compute heat index
    float computeHeatIndex(float temperature, float humidity);
};

#endif // HEAT_INDEX_H
