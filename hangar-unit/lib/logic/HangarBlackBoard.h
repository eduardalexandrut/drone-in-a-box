#ifndef DRONE_IN_A_BOX_HANGARBLACKBOARD_H
#define DRONE_IN_A_BOX_HANGARBLACKBOARD_H

#include "HangarTypes.h"

class HangarBlackBoard {
public:
    DoorState getDoorState() const { return doorState; }
    DoorCommand getDoorCommand() const { return doorCommand; }
    int getDoorPosition() const { return doorPosition; }

    void setDoorState(DoorState doorState) { this->doorState = doorState; }
    void setDoorCommand(DoorCommand doorCommand) { this->doorCommand = doorCommand; }
    void setDoorPosition(int doorPosition) { this->doorPosition = doorPosition; }

private:
    DoorState doorState = DoorState::CLOSED;
    DoorCommand doorCommand = DoorCommand::CLOSE_DOOR;
    int doorPosition = 0;   // 0 = closed, 100 = open
};

#endif //DRONE_IN_A_BOX_HANGARBLACKBOARD_H