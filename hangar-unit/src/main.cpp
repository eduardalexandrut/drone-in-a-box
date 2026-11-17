#include <Arduino.h>

#include "HangarTimer.h"
#include "BlinkTask.h"
#include "Scheduler.h"

BlinkTask blink(LED_BUILTIN);
BlinkTask blink2(15);
HangarTimer timer;
Scheduler scheduler(timer);
unsigned long last = 0;

void setup() {
  Serial.begin(115200);

  blink.begin();
  blink.init(500);
  blink2.begin();
  blink2.init(100);

  scheduler.init(50);
  scheduler.addTask(&blink);
  scheduler.addTask(&blink2);
}

void loop() {
  scheduler.schedule();
}