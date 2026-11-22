#include <Arduino.h>
#include "Scheduler.h"
#include "HangarBlackBoard.h"
#include "DoorTask.h"
#include "ServoMotor.h"
#include "DoorToggleTask.h"

#include "BusyWaitTimer.h"
#include "BlinkTask.h"
#include "Scheduler.h"

// BlinkTask blink(LED_BUILTIN);
// BlinkTask blink2(15);
BusyWaitTimer timer;
// Scheduler scheduler(timer);
// unsigned long last = 0;

const int SERVO_PIN = 16;

Scheduler scheduler(timer);
HangarBlackBoard bb;

ServoMotor servo(SERVO_PIN);
DoorToggleTask toggle(bb);
DoorTask door(bb, servo, 360, 3);   // 90° open, 3° per tick: 1.5 s at 50 ms



void setup() {
  Serial.begin(115200);
  servo.begin();
  door.begin();

  toggle.init(5000);   // switches the command every 5 s
  door.init(50);

  scheduler.init(50);
  scheduler.addTask(&toggle);
  scheduler.addTask(&door);
}

void loop() {
  scheduler.schedule();
}