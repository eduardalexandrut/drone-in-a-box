//
// Created by Eduard on 24/11/2026.
//

#ifndef DRONE_IN_A_BOX_BUZZER_H
#define DRONE_IN_A_BOX_BUZZER_H
#include "IBuzzer.h"
#include <Arduino.h>

class Buzzer : public IBuzzer {
    public:
        explicit Buzzer(int pin): pin(pin) {}

        void begin() {
            pinMode(pin, OUTPUT);
            set(false);
        }

        void set(bool isOn) override {
            digitalWrite(pin, isOn ? HIGH : LOW);
        }

private:
    int pin;
};
#endif //DRONE_IN_A_BOX_BUZZER_H
