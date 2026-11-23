//
// Created by aedua on 11/23/2026.
//

#ifndef DRONE_IN_A_BOX_IRSENSOR_H
#define DRONE_IN_A_BOX_IRSENSOR_H
#include <Arduino.h>
#include "IPresenceSensor.h"

class IrSensor : public IPresenceSensor
{
public:
    explicit IrSensor(int pin, bool activeLow):
        pin(pin), activeLow(activeLow) {}

    void begin() {
        pinMode(pin, INPUT);
    };

    bool isDetected() override {
        const int reading = digitalRead(pin);
        return activeLow ? (reading == LOW) : (reading == HIGH);
    };

private:
    int pin;
    bool activeLow;
};

#endif //DRONE_IN_A_BOX_IRSENSOR_H
