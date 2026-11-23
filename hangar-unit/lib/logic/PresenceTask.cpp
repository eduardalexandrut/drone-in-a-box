//
// Created by aedua on 11/23/2026.
//

#include "PresenceTask.h"

PresenceTask::PresenceTask(HangarBlackBoard& bb, IPresenceSensor& presenceSensor):
    bb(bb),
    presenceSensor(presenceSensor),
    filter(false, READINGS_TO_PRESENT, READINGS_TO_ABSENT) {}

void PresenceTask::tick()
{
    const bool isDetected = presenceSensor.isDetected();
    bb.setDroneDetected(filter.update(isDetected));
}

void PresenceTask::begin()
{
    const bool isDetected = presenceSensor.isDetected();
    filter.reset(isDetected);
    bb.setDroneDetected(isDetected);

}
