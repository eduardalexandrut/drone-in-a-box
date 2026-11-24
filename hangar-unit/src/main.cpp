#include <Arduino.h>
#include "Scheduler.h"
#include "HangarBlackBoard.h"
#include "DoorTask.h"
#include "ServoMotor.h"
#include "examples/DoorToggleTask.h"

#include "BusyWaitTimer.h"
#include "Buzzer.h"
#include "Dht11Sensor.h"
#include "EnvMonitorTask.h"
#include "IrSensor.h"
#include "RfidReader.h"
#include "examples/EnvPrintTask.h"
#include "SafetyPolicy.h"
#include "examples/BlinkTask.h"
#include "Scheduler.h"
#include "TrafficLight.h"
#include "examples/StateCycleTask.h"
#include "StatusTask.h"
#include "examples/UidPrintTask.h"
#include "examples/PresencePrintTask.h"
#include "../lib/logic/PresenceTask.h"

// BlinkTask blink(LED_BUILTIN);
// BlinkTask blink2(15);
BusyWaitTimer timer;

const int GREEN_PIN = 12;
const int YELLOW_PIN = 11;
const int RED_PIN = 10;
const int BUZZER_PIN = 17;

Scheduler scheduler(timer);
HangarBlackBoard bb;

// TrafficLight light(GREEN_PIN, YELLOW_PIN, RED_PIN);
// Buzzer buzzer(BUZZER_PIN);
// StateCycleTask stateCycle(bb);
// StatusTask task(bb, light, buzzer);

// rfid
const uint8_t RFID_ADDRESS = 0x28;
const int RFID_RESET_PIN = 22;

RfidReader rfidReader(RFID_ADDRESS, RFID_RESET_PIN);
UidPrintTask uidPrint(rfidReader);

// DTH
// const int DHT_SENSOR = 17;
// Dht11Sensor dhtSensor(DHT_SENSOR);
// SafetyPolicy policy(0, 40, 85);
// EnvMonitorTask envMonitorTask(bb, dhtSensor, policy);
// EnvPrintTask envPrintTask(bb);
//
// // Presence
// const int IR_PIN = 18;
// IrSensor irSensor(IR_PIN, true);
// PresenceTask presenceTask(bb, irSensor);
// PresencePrintTask presencePrintTask(bb);


void setup() {
  Serial.begin(115200);

  // Trafficlight
  // light.begin();
  // buzzer.begin();
  // stateCycle.init(2000);
  // task.init(250);
  //
  // // DHT sensor
  // dhtSensor.begin();
  // envMonitorTask.init(2000);
  // envPrintTask.init(2000);

  // IR
  // irSensor.begin();
  // presenceTask.begin();
  // presenceTask.init(100);
  // presencePrintTask.init(100);
  //

  // RFID
  rfidReader.begin();
  uidPrint.init(200);
  scheduler.init(50);
  // scheduler.addTask(&presenceTask);
  // scheduler.addTask(&presencePrintTask);
  // scheduler.addTask(&stateCycle);
  // scheduler.addTask(&task);
  //
  // scheduler.addTask(&envMonitorTask);
  // scheduler.addTask(&envPrintTask);
  scheduler.addTask(&uidPrint);
}

void loop() {
  scheduler.schedule();
}