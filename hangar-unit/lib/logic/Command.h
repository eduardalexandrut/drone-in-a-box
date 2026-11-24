//
// Created by Eduard on 24/11/2026.
//

#ifndef DRONE_IN_A_BOX_COMMAND_H
#define DRONE_IN_A_BOX_COMMAND_H
enum CommandType {
    INVALID,
    HELP,
    STATUS,
    DOOR_OPEN,
    DOOR_CLOSE,
    ENV_FORCE,        // args: temperature, humidity
    ENV_REAL,
    PRESENCE_FORCE,   // args: 1 = present, 0 = absent
    PRESENCE_REAL
};
struct Command {
    static const int MAX_ARGS = 2;
    
    CommandType type = INVALID;
    int args[MAX_ARGS] = {};
    int argCount = 0;
};
#endif //DRONE_IN_A_BOX_COMMAND_H
