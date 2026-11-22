//
// Created by Eduard on 22/11/2026.
//

#ifndef DRONE_IN_A_BOX_DOORTASK_H
#define DRONE_IN_A_BOX_DOORTASK_H
#include "AbstractTask.h"
#include "HangarBlackBoard.h"
#include "IServo.h"

class DoorTask : public AbstractTask {
public:
    DoorTask(HangarBlackBoard &bb, IServo &servo, int openAngle, int step);

    void tick() override;
    void begin();

    DoorState getDoorState() const {return doorState;};
    int getAngle() const {return angle;};
private:
    void applyOutputs();

    HangarBlackBoard& bb;
    IServo& servo;
    const int openAngle;
    const int step;

    DoorState doorState = DoorState::CLOSED;
    int angle = 0;
    int lastWritten = -1;
};

#endif //DRONE_IN_A_BOX_DOORTASK_H
