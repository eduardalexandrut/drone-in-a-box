//
// Created by Eduard on 22/11/2026.
//

#ifndef DRONE_IN_A_BOX_STATUSTASK_H
#define DRONE_IN_A_BOX_STATUSTASK_H
#include "AbstractTask.h"
#include "HangarBlackBoard.h"
#include "IBuzzer.h"
#include "ITrafficLight.h"

class StatusTask: public AbstractTask {
public:
    StatusTask(HangarBlackBoard &bb, ITrafficLight &light, IBuzzer& buzzer);
    void tick() override;
private:
    enum Signal {OFF, ON, BLINK};

    struct Pattern {
        Signal green;
        Signal yellow;
        Signal red;
        Signal buzzer;
    };

    static Pattern patternFor(HangarState state);
    static bool isLit(Signal signal, bool phase);

    HangarBlackBoard &bb;
    ITrafficLight &light;
    IBuzzer &buzzer;
    bool phase = false;
};

#endif //DRONE_IN_A_BOX_STATUSTASK_H
