//
// Created by Eduard on 22/11/2026.
//

#ifndef DRONE_IN_A_BOX_TRAFFICLIGHT_H
#define DRONE_IN_A_BOX_TRAFFICLIGHT_H

#include <Arduino.h>
#include "ITrafficLight.h"

class TrafficLight : public ITrafficLight {
public:
    explicit TrafficLight(int greenPin, int yellowPin, int redPin): greenPin(greenPin), yellowPin(yellowPin), redPin(redPin) {}

    void begin() {
        pinMode(greenPin, OUTPUT);
        pinMode(yellowPin, OUTPUT);
        pinMode(redPin, OUTPUT);
        set(false, false, false);
    }

    void set(bool green, bool yellow, bool red) {
        digitalWrite(greenPin, green ? HIGH : LOW);
        digitalWrite(yellowPin, yellow ? HIGH : LOW);
        digitalWrite(redPin, red ? HIGH : LOW);
    }

private:
    int greenPin, yellowPin, redPin;
};

#endif //DRONE_IN_A_BOX_TRAFFICLIGHT_H
