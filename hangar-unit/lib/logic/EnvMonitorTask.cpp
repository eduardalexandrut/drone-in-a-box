//
// Created by Eduard on 22/11/2026.
//
#include "EnvMonitorTask.h"

#include "SafetyPolicy.h"

EnvMonitorTask::EnvMonitorTask(HangarBlackBoard &bb, IEnvSensor &envSensor, const SafetyPolicy &safetyPolicy):
    bb(bb),
    envSensor(envSensor),
    safetyPolicy(safetyPolicy),
    filter(false, READINGS_TO_SAFE, READINGS_TO_UNSAFE) {   // starts unsafe
        bb.setEnvOk(filter.get());
    }


void EnvMonitorTask::tick() {
    EnvReading reading;
    const bool isValid = envSensor.read(reading);

    if (isValid) {
        bb.setTemperature(reading.temperature);
        bb.setHumidity(reading.humidity);
    }

    // a failed reading counts as unsafe (EC-12)
    const bool safeNow = isValid && safetyPolicy.isSafe(reading);
    bb.setEnvOk(filter.update(safeNow));
}
