//
// Created by Eduard on 24/11/2026.
//
#include <string.h>
#include "CommandParser.h"

#include <cstdlib>

#include "Command.h"
static Command makeCommand(CommandType type) {
    Command command;
    command.type = type;
    return command;
}

static Command makeCommand(CommandType type, int arg) {
    Command command = makeCommand(type);
    command.args[0] = arg;
    command.argCount = 1;
    return command;
}

static Command makeCommand(CommandType type, int arg0, int arg1) {
    Command command = makeCommand(type);
    command.args[0] = arg0;
    command.args[1] = arg1;
    command.argCount = 2;
    return command;
}

static bool equals(const char* a, const char* b) {
    return strcmp(a, b) == 0;
}

// Converts text to an integer; fails if the text is not exactly a number.
static bool parseInt(const char* text, int& out) {
    char* end = nullptr;
    const long value = strtol(text, &end, 10);
    if (end == text || *end != '\0') {
        return false;
    }
    out = static_cast<int>(value);
    return true;
}


Command CommandParser::parse(const char* line) {
    Command invalid;   // type is INVALID by default

    if (line == nullptr) {
        return invalid;
    }

    // copy the line: strtok modifies the string it splits
    char buffer[BUFFER_SIZE];
    strncpy(buffer, line, BUFFER_SIZE - 1);
    buffer[BUFFER_SIZE - 1] = '\0';

    // split into words
    const char* delimiters = " \t\r\n";
    char* words[MAX_WORDS] = {};
    int count = 0;

    char* word = strtok(buffer, delimiters);
    while (word != nullptr) {
        if (count == MAX_WORDS) {
            return invalid;   // too many words for any known command
        }
        words[count++] = word;
        word = strtok(nullptr, delimiters);
    }

    if (count == 0) {
        return invalid;
    }

    // recognise the command
    const char* name = words[0];

    if (equals(name, "help") && count == 1) {
        return makeCommand(CommandType::HELP);
    }

    if (equals(name, "status") && count == 1) {
        return makeCommand(CommandType::STATUS);
    }

    if (equals(name, "door") && count == 2) {
        if (equals(words[1], "open"))  return makeCommand(CommandType::DOOR_OPEN);
        if (equals(words[1], "close")) return makeCommand(CommandType::DOOR_CLOSE);
        return invalid;
    }

    if (equals(name, "env")) {
        if (count == 2 && equals(words[1], "real")) {
            return makeCommand(CommandType::ENV_REAL);
        }
        int temperature = 0;
        int humidity = 0;
        if (count == 3 && parseInt(words[1], temperature) && parseInt(words[2], humidity)) {
            return makeCommand(CommandType::ENV_FORCE, temperature, humidity);
        }
        return invalid;
    }

    if (equals(name, "presence") && count == 2) {
        if (equals(words[1], "on"))   return makeCommand(CommandType::PRESENCE_FORCE, 1);
        if (equals(words[1], "off"))  return makeCommand(CommandType::PRESENCE_FORCE, 0);
        if (equals(words[1], "real")) return makeCommand(CommandType::PRESENCE_REAL);
        return invalid;
    }


    return invalid;
}
