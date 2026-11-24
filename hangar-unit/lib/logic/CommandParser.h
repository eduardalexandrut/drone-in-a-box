//
// Created by Eduard on 24/11/2026.
//

#ifndef DRONE_IN_A_BOX_COMMANDPARSER_H
#define DRONE_IN_A_BOX_COMMANDPARSER_H
#include <cstring>

#include "Command.h"

class CommandParser {
public:
    static Command parse(const char* line);

private:
    static const int BUFFER_SIZE = 64;
    static const int MAX_WORDS = 4;
};
#endif //DRONE_IN_A_BOX_COMMANDPARSER_H
