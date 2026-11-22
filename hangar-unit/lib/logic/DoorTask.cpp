//
// Created by Eduard on 22/11/2026.
//
#include "DoorTask.h"

DoorTask::DoorTask(HangarBlackBoard& bb, IServo& servo, int openAngle, int step)
  : bb(bb), servo(servo), openAngle(openAngle), step(step) {}

void DoorTask::begin() {
  doorState = DoorState::CLOSED;
  angle = 0;
  applyOutputs();
};

void DoorTask::tick() {
  const DoorCommand cmd = bb.getDoorCommand();

  switch (doorState) {
    case DoorState::CLOSED:
      if (cmd == DoorCommand::OPEN_DOOR) doorState = DoorState::OPENING;
      break;

    case DoorState::OPENING:
      if (cmd == DoorCommand::CLOSE_DOOR) {
        doorState = DoorState::CLOSING;
      } else {
        angle += step;
        if (angle >= openAngle) {
          angle = openAngle;
          doorState = DoorState::OPEN;
        }
      }
      break;

    case DoorState::OPEN:
      if (cmd == DoorCommand::CLOSE_DOOR) doorState = DoorState::CLOSING;
      break;

    case DoorState::CLOSING:
      if (cmd == DoorCommand::OPEN_DOOR) {
        doorState = DoorState::OPENING;
      } else {
        angle -= step;
        if (angle <= 0) {
          angle = 0;
          doorState = DoorState::CLOSED;
        }
      }
      break;
  }

  applyOutputs();
}

void DoorTask::applyOutputs() {
  if (angle != lastWritten) {
    servo.setAngle(angle);
    lastWritten = angle;
  }
  bb.setDoorState(doorState);
  bb.setDoorPosition(angle * 100 / openAngle);
}