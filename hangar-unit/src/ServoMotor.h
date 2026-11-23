//
// Created by Eduard on 22/11/2026.
//

#ifndef DRONE_IN_A_BOX_SERVOMOTOR_H
#define DRONE_IN_A_BOX_SERVOMOTOR_H
#include <Arduino.h>
#include <Servo.h>
#include "IServo.h"

class ServoMotor: public IServo {
public:
    explicit ServoMotor(int pin): pin(pin) {};

    void begin() { servo.attach(pin); }

    void setAngle(int degrees) override{ servo.write(degrees); };

private:
    int pin;
    Servo servo;
};


#endif //DRONE_IN_A_BOX_SERVOMOTOR_H
