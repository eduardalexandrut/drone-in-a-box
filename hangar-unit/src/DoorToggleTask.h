//
// Created by Eduard on 22/11/2026.
//

#ifndef DRONE_IN_A_BOX_DOORTOGGLETASK_H
#define DRONE_IN_A_BOX_DOORTOGGLETASK_H
#include "AbstractTask.h"
#include "../lib/logic/HangarBlackBoard.h"


class DoorToggleTask : public AbstractTask {
public:
    explicit DoorToggleTask(HangarBlackBoard& bb) : bb(bb) {}

    void tick() override {
        open = !open;
        bb.setDoorCommand(open ? DoorCommand::OPEN_DOOR : DoorCommand::CLOSE_DOOR);
    }

private:
    HangarBlackBoard& bb;
    bool open = false;
};
#endif //DRONE_IN_A_BOX_DOORTOGGLETASK_H
