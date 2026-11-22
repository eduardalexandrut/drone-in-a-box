//
// Created by Eduard on 22/11/2026.
//

#ifndef DRONE_IN_A_BOX_STATECYCLETASK_H
#define DRONE_IN_A_BOX_STATECYCLETASK_H
// src/StateCycleTask.h
#ifndef STATECYCLETASK_H
#define STATECYCLETASK_H

#include <Arduino.h>
#include "AbstractTask.h"
#include "HangarBlackBoard.h"

class StateCycleTask : public AbstractTask {
public:
    explicit StateCycleTask(HangarBlackBoard& bb) : bb(bb) {}

    void tick() override {
        static const HangarState states[] = {
            HangarState::IDLE, HangarState::TAKEOFF_PREP, HangarState::DEPARTURE,
            HangarState::DRONE_OUT, HangarState::AUTHENTICATING, HangarState::LANDING_PREP,
            HangarState::LANDING_WAIT, HangarState::DENYING, HangarState::ALARM
        };
        bb.setHangarState(states[index]);
        Serial.print("hangarState = ");
        Serial.println(toString(states[index]));
        index = (index + 1) % 9;
    }

private:
    HangarBlackBoard& bb;
    int index = 0;
};

#endif

#endif //DRONE_IN_A_BOX_STATECYCLETASK_H
