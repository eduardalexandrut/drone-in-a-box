//
// Created by Eduard on 22/11/2026.
//

#ifndef DRONE_IN_A_BOX_STATUSTASK_H
#define DRONE_IN_A_BOX_STATUSTASK_H
#include "AbstractTask.h"
#include "HangarBlackBoard.h"
#include "ITrafficLight.h"

class StatusTask: public AbstractTask {
public:
    StatusTask(HangarBlackBoard &bb, ITrafficLight &light);
    void tick() override;
private:
    enum Lamp {OFF, ON, BLINK};

    struct Pattern {
        Lamp green;
        Lamp yellow;
        Lamp red;
    };

    static Pattern patternFor(HangarState state);
    static bool isLit(Lamp lamp, bool phase);

    HangarBlackBoard &bb;
    ITrafficLight &light;
    bool phase = false;
};

#endif //DRONE_IN_A_BOX_STATUSTASK_H
