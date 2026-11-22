// BlinkTask.cpp
#include "BlinkTask.h"

#include <Arduino.h>

BlinkTask::BlinkTask(int pin) : pin(pin) {}

void BlinkTask::begin() {
    pinMode(pin, OUTPUT);
}

void BlinkTask::tick() {
    on = !on;
    digitalWrite(pin, on ? HIGH : LOW);
}