#include <Arduino.h>
#include "Scheduler.h"
#include "HangarBlackBoard.h"
#include "DoorTask.h"
#include "ServoMotor.h"
#include "examples/DoorToggleTask.h"

#include "BusyWaitTimer.h"
#include "examples/BlinkTask.h"
#include "Scheduler.h"
#include "TrafficLight.h"
#include "examples/StateCycleTask.h"
#include "StatusTask.h"

// BlinkTask blink(LED_BUILTIN);
// BlinkTask blink2(15);
BusyWaitTimer timer;
// Scheduler scheduler(timer);
// unsigned long last = 0;

const int GREEN_PIN = 12;
const int YELLOW_PIN = 11;
const int RED_PIN = 10;

Scheduler scheduler(timer);
HangarBlackBoard bb;

TrafficLight light(GREEN_PIN, YELLOW_PIN, RED_PIN);
StateCycleTask stateCycle(bb);
StatusTask task(bb, light);


void setup() {
  Serial.begin(115200);

  light.begin();
  stateCycle.init(2000);
  task.init(250);


  scheduler.init(50);
  scheduler.addTask(&stateCycle);
  scheduler.addTask(&task);
}

void loop() {
  scheduler.schedule();
}