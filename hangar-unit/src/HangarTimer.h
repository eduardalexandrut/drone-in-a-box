//
// Created by Eduard on 17/11/2026.
//

#ifndef HANGAR_UNIT_ARDUINOTIMER_H
#define HANGAR_UNIT_ARDUINOTIMER_H
#pragma once
#include <Arduino.h>
#include "ITimer.h"

class HangarTimer : public ITimer {
public:
    void setupPeriod(unsigned long periodMs) override {
        period = periodMs;
        next = millis() + period;
    }

    bool waitForNextTick() override {
        bool onTime = (long)(millis() - next) < 0;
        while ((long)(millis() - next) < 0) {
            // busy wait until the start of the next period
        }
        // move to the next slot of the grid that is still in the future,
        // skipping the ticks that were lost during an overrun
        do {
            next += period;
        } while ((long)(millis() - next) >= 0);
        return onTime;
    }

private:
    unsigned long period = 0;
    unsigned long next = 0;
};
#endif //HANGAR_UNIT_ARDUINOTIMER_H
