//
// Created by Eduard on 22/11/2026.
//
#include "StatusTask.h"

StatusTask::StatusTask(HangarBlackBoard &bb, ITrafficLight &light, IBuzzer &buzzer):
    bb(bb), light(light), buzzer(buzzer) {}

void StatusTask::tick() {
    phase = !phase;
    const Pattern pattern = patternFor(bb.getHangarState());
    light.set(isLit(pattern.green, phase), isLit(pattern.yellow, phase), isLit(pattern.red, phase));
    buzzer.set(isLit(pattern.buzzer, phase));
}

bool StatusTask::isLit(Signal lamp, bool phase) {
    return lamp == Signal::ON || (lamp == Signal::BLINK && phase);
}

// Mapping from behaviour 4.6
StatusTask::Pattern StatusTask::patternFor(HangarState state) {
    switch (state) {
        case HangarState::IDLE:           return { Signal::ON,    Signal::OFF,   Signal::OFF,   Signal::OFF };
        case HangarState::TAKEOFF_PREP:
        case HangarState::LANDING_PREP:
        case HangarState::AUTHENTICATING: return { Signal::OFF,   Signal::BLINK, Signal::OFF,   Signal::OFF };
        case HangarState::DEPARTURE:
        case HangarState::LANDING_WAIT:   return { Signal::OFF,   Signal::ON,    Signal::OFF,   Signal::OFF };
        case HangarState::DRONE_OUT:      return { Signal::BLINK, Signal::OFF,   Signal::OFF,   Signal::OFF };
        case HangarState::DENYING:        return { Signal::OFF,   Signal::OFF,   Signal::ON,   Signal::OFF  };
        case HangarState::ALARM:          return { Signal::OFF,   Signal::OFF,   Signal::BLINK, Signal::BLINK };
    }
    return { Signal::OFF, Signal::OFF, Signal::ON , Signal::ON};   // unknown state: red, to be safe
}