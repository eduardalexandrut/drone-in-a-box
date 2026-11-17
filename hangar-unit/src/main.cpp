#include <Arduino.h>
#include "BlinkTask.h"

BlinkTask blink(LED_BUILTIN);
unsigned long last = 0;

void setup() {
  Serial.begin(115200);
  blink.begin();
}

void loop() {
  if (millis() - last >= 100) {
    last = millis();
    blink.tick();
    Serial.println("tick");
  }
}