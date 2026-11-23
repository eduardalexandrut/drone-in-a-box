//
// Created by Eduard on 22/11/2026.
//
#include "StatusTask.h"

StatusTask::StatusTask(HangarBlackBoard &bb, ITrafficLight &light):
    bb(bb), light(light) {}

void StatusTask::tick() {
    phase = !phase;
    const Pattern pattern = patternFor(bb.getHangarState());
    light.set(isLit(pattern.green, phase), isLit(pattern.yellow, phase), isLit(pattern.red, phase));
}

bool StatusTask::isLit(Lamp lamp, bool phase) {
    return lamp == Lamp::ON || (lamp == Lamp::BLINK && phase);
}

// Mapping from behaviour 4.6: { green, yellow, red }
StatusTask::Pattern StatusTask::patternFor(HangarState state) {
    switch (state) {
        case HangarState::IDLE:           return { Lamp::ON,    Lamp::OFF,   Lamp::OFF };
        case HangarState::TAKEOFF_PREP:
        case HangarState::LANDING_PREP:
        case HangarState::AUTHENTICATING: return { Lamp::OFF,   Lamp::BLINK, Lamp::OFF };
        case HangarState::DEPARTURE:
        case HangarState::LANDING_WAIT:   return { Lamp::OFF,   Lamp::ON,    Lamp::OFF };
        case HangarState::DRONE_OUT:      return { Lamp::BLINK, Lamp::OFF,   Lamp::OFF };
        case HangarState::DENYING:        return { Lamp::OFF,   Lamp::OFF,   Lamp::ON  };
        case HangarState::ALARM:          return { Lamp::OFF,   Lamp::OFF,   Lamp::BLINK };
    }
    return { Lamp::OFF, Lamp::OFF, Lamp::ON };   // unknown state: red, to be safe
}