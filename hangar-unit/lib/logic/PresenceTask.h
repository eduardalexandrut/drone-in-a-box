//
// Created by aedua on 11/23/2026.
//

#ifndef DRONE_IN_A_BOX_PRESENCETASK_H
#define DRONE_IN_A_BOX_PRESENCETASK_H
#include "AbstractTask.h"
#include "ConsecutiveFilter.h"
#include "HangarBlackBoard.h"
#include "IPresenceSensor.h"

class PresenceTask : public AbstractTask
{
public:
    PresenceTask(HangarBlackBoard& bb, IPresenceSensor& presenceSensor);

    void tick() override;
    void begin();

private:
    static const int READINGS_TO_PRESENT = 3;
    static const int READINGS_TO_ABSENT = 3;

    HangarBlackBoard& bb;
    IPresenceSensor& presenceSensor;
    ConsecutiveFilter filter;
};

#endif //DRONE_IN_A_BOX_PRESENCETASK_H
