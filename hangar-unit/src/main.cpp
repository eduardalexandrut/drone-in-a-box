#include <Arduino.h>
#include "Scheduler.h"
#include "HangarBlackBoard.h"
#include "DoorTask.h"
#include "ServoMotor.h"
#include "examples/DoorToggleTask.h"

#include "BusyWaitTimer.h"
#include "Dht11Sensor.h"
#include "EnvMonitorTask.h"
#include "examples/EnvPrintTask.h"
#include "SafetyPolicy.h"
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

// DTH
const int DHT_SENSOR = 17;
Dht11Sensor dhtSensor(DHT_SENSOR);
SafetyPolicy policy(0, 40, 85);
EnvMonitorTask envMonitorTask(bb, dhtSensor, policy);
EnvPrintTask envPrintTask(bb);


void setup() {
  Serial.begin(115200);

  // Trafficlight
  light.begin();
  stateCycle.init(2000);
  task.init(250);

  // DHT sensor
  dhtSensor.begin();
  envMonitorTask.init(2000);
  envPrintTask.init(2000);

  scheduler.init(50);
  scheduler.addTask(&stateCycle);
  scheduler.addTask(&task);

  scheduler.addTask(&envMonitorTask);
  scheduler.addTask(&envPrintTask);
}

void loop() {
  scheduler.schedule();
}