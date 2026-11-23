// EnvMonitorTask.h
#ifndef DRONE_IN_A_BOX_ENVMONITORTASK_H
#define DRONE_IN_A_BOX_ENVMONITORTASK_H

#include "AbstractTask.h"
#include "ConsecutiveFilter.h"
#include "HangarBlackBoard.h"
#include "IEnvSensor.h"
#include "SafetyPolicy.h"

class EnvMonitorTask : public AbstractTask {
public:
    EnvMonitorTask(HangarBlackBoard& bb, IEnvSensor& envSensor, const SafetyPolicy& safetyPolicy);

    void tick() override;

private:
    static const int READINGS_TO_SAFE = 3;
    static const int READINGS_TO_UNSAFE = 2;

    HangarBlackBoard& bb;
    IEnvSensor& envSensor;
    const SafetyPolicy& safetyPolicy;
    ConsecutiveFilter filter;   // a value, not a reference: it belongs to this task
};

#endif //DRONE_IN_A_BOX_ENVMONITORTASK_H