//
// Created by Eduard on 22/11/2026.
//

// src/example/EnvPrintTask.h
#ifndef DRONE_IN_A_BOX_ENVPRINTTASK_H
#define DRONE_IN_A_BOX_ENVPRINTTASK_H

#include <Arduino.h>
#include "AbstractTask.h"
#include "HangarBlackBoard.h"

class EnvPrintTask : public AbstractTask {
public:
    explicit EnvPrintTask(HangarBlackBoard& bb) : bb(bb) {}

    void tick() override {
        Serial.print("temperature: ");
        Serial.print(bb.getTemperature());
        Serial.print(" C, humidity: ");
        Serial.print(bb.getHumidity());
        Serial.print(" %, envOk: ");
        Serial.println(bb.isEnvOk() ? "true" : "false");
    }

private:
    HangarBlackBoard& bb;
};

#endif //DRONE_IN_A_BOX_ENVPRINTTASK_H