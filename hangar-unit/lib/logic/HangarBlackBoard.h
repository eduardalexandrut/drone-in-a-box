#ifndef DRONE_IN_A_BOX_HANGARBLACKBOARD_H
#define DRONE_IN_A_BOX_HANGARBLACKBOARD_H

#include "Event.h"
#include "HangarTypes.h"

class HangarBlackBoard {
public:
    DoorState getDoorState() const { return doorState; }
    DoorCommand getDoorCommand() const { return doorCommand; }
    int getDoorPosition() const { return doorPosition; }
    HangarState getHangarState() const { return hangarState; }
    int getTemperature() const { return temperature; }
    int getHumidity() const { return humidity; }
    bool isEnvOk() const { return envOk; }
    bool isDroneDetected() const { return droneDetected; }
    Event<bool>& resetFromBadge() { return resetFromBadgeEvent; }

    void setDoorState(DoorState doorState) { this->doorState = doorState; }
    void setDoorCommand(DoorCommand doorCommand) { this->doorCommand = doorCommand; }
    void setDoorPosition(int doorPosition) { this->doorPosition = doorPosition; }
    void setHangarState(HangarState hangarState) { this->hangarState = hangarState; }
    void setTemperature(int temperature) { this->temperature = temperature; }
    void setHumidity(int humidity) { this->humidity = humidity; }
    void setEnvOk(bool envOk) { this->envOk = envOk; }
    void setDroneDetected(bool droneDetected) { this->droneDetected = droneDetected; }

private:
    DoorState doorState = DoorState::CLOSED;
    DoorCommand doorCommand = DoorCommand::CLOSE_DOOR;
    int doorPosition = 0;   // 0 = closed, 100 = open
    HangarState hangarState = HangarState::IDLE;
    int temperature = 0;
    int humidity = 0;
    bool envOk = false;
    bool droneDetected = false;
    Event<bool> resetFromBadgeEvent;// a reference, so producer and consumer work on the same object
};

#endif //DRONE_IN_A_BOX_HANGARBLACKBOARD_H