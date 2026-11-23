//
// Created by aedua on 11/23/2026.
//

#ifndef DRONE_IN_A_BOX_PRESENCEPRINTTASK_H
#define DRONE_IN_A_BOX_PRESENCEPRINTTASK_H

#include <Arduino.h>
#include "AbstractTask.h"
#include "HangarBlackBoard.h"

class PresencePrintTask : public AbstractTask {
    public:
    explicit PresencePrintTask(HangarBlackBoard& bb) : bb(bb) {}

    void tick() override {
        const bool detected = bb.isDroneDetected();
        if (first || detected != lastPrinted) {
            Serial.print("droneDetected = ");
            Serial.println(detected ? "true" : "false");
            lastPrinted = detected;
            first = false;
        }
    }

    private:
    HangarBlackBoard& bb;
    bool lastPrinted = false;
    bool first = true;   // always print the first value
};

#endif //DRONE_IN_A_BOX_PRESENCEPRINTTASK_H
