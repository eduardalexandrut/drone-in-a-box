//
// Created by Eduard on 24/11/2026.
//

#ifndef DRONE_IN_A_BOX_OVERRIDABLEPRESENCESENSOR_H
#define DRONE_IN_A_BOX_OVERRIDABLEPRESENCESENSOR_H
#include "IPresenceSensor.h"

class OverridablePresenceSensor : public IPresenceSensor {
public:
    explicit OverridablePresenceSensor(IPresenceSensor& realSensor) : realSensor(realSensor) {}

    void force(bool detected) {
        forcedValue = detected;
        forced = true;
    }

    void clear() {
        forced = false;
    }

    bool isDetected() override {
        if (forced) {
            return forcedValue;
        }
        return realSensor.isDetected();
    }

private:
    IPresenceSensor& realSensor;
    bool forcedValue = false;
    bool forced = false;
};

#endif //DRONE_IN_A_BOX_OVERRIDABLEPRESENCESENSOR_H
