//
// Created by Eduard on 22/11/2026.
//

#ifndef DRONE_IN_A_BOX_HANGARTYPES_H
#define DRONE_IN_A_BOX_HANGARTYPES_H
#pragma once

enum DoorCommand { OPEN_DOOR, CLOSE_DOOR };
enum DoorState { CLOSING, CLOSED, OPENING, OPEN };

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

#endif //DRONE_IN_A_BOX_HANGARTYPES_H
