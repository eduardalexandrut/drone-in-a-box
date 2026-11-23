//
// Created by Eduard on 22/11/2026.
//

#ifndef DRONE_IN_A_BOX_HANGARTYPES_H
#define DRONE_IN_A_BOX_HANGARTYPES_H
#pragma once

enum DoorCommand { OPEN_DOOR, CLOSE_DOOR };
enum DoorState { CLOSING, CLOSED, OPENING, OPEN };
enum HangarState {
    IDLE, TAKEOFF_PREP, DEPARTURE, DRONE_OUT, AUTHENTICATING, LANDING_PREP,
    LANDING_WAIT, DENYING, ALARM
};

inline const char* toString( DoorState doorState ) {
    switch( doorState ) {
        case DoorState::CLOSING:
            return "CLOSING";
        case DoorState::OPEN:
            return "OPEN";
        case DoorState::CLOSED:
            return "CLOSED";
        case DoorState::OPENING:
            return "OPENING";
    }
    return "?";
};

inline const char* toString( HangarState hangarState ) {
    switch( hangarState ) {
        case HangarState::IDLE:
            return "IDLE";
        case HangarState::TAKEOFF_PREP:
            return "TAKEOFF_PREP";
        case HangarState::DEPARTURE:
            return "DEPARTURE";
        case HangarState::DRONE_OUT:
            return "DRONE_OUT";
        case HangarState::AUTHENTICATING:
            return "AUTHENTICATING";
        case HangarState::LANDING_PREP:
            return "LANDING_PREP";
        case HangarState::LANDING_WAIT:
            return "LANDING_WAIT";
        case HangarState::DENYING:
            return "DENYING";
        case HangarState::ALARM:
            return "ALARM";
    }
    return "?";
}

#endif //DRONE_IN_A_BOX_HANGARTYPES_H
