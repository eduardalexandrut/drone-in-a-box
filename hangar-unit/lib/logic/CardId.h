//
// Created by Eduard on 24/11/2026.
//

#ifndef DRONE_IN_A_BOX_CARDID_H
#define DRONE_IN_A_BOX_CARDID_H
#include <stdint.h>

struct CardId {
    static const int MAX_LENGTH = 10;
    uint8_t uid[MAX_LENGTH] = {};
    int length;

    bool operator==(const CardId &other) const {
        if (length != other.length) return false;
        for (int i = 0; i < length; i++) {
            if (uid[i] != other.uid[i]) return false;
        }

        return true;
    }
};
#endif //DRONE_IN_A_BOX_CARDID_H
