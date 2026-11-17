//
// Created by Eduard on 17/11/2026.
//

#ifndef DRONE_IN_A_BOX_BLINKTASK_H
#define DRONE_IN_A_BOX_BLINKTASK_H


// BlinkTask.h
#pragma once
#include "../../common/src/AbstractTask.h"

class BlinkTask : public AbstractTask {
public:
    explicit BlinkTask(int pin);
    void begin();
    void tick() override;

private:
    int pin;
    bool on = false;
};
#endif //DRONE_IN_A_BOX_BLINKTASK_H
