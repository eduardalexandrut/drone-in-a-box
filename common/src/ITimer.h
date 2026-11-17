//
// Created by Eduard on 17/11/2026.
//

#ifndef DRONE_IN_A_BOX_ITIMER_H
#define DRONE_IN_A_BOX_ITIMER_H

#pragma once

class ITimer {
public:
    virtual ~ITimer() = default;
    virtual void setupPeriod(unsigned long periodMs) = 0;

    // Waits until the start of the next period.
    // Returns false if the period had already expired (overrun).
    virtual bool waitForNextTick() = 0;
};

#endif //DRONE_IN_A_BOX_ITIMER_H
