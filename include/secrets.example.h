//
// Created by Eduard on 24/11/2026.
//

#ifndef DRONE_IN_A_BOX_SECRETS_EXAMPLE_H
#define DRONE_IN_A_BOX_SECRETS_EXAMPLE_H
#include "CardId.h"
static const CardId AUTHORIZED_CARDS[] = {
    { { 0x00, 0x00, 0x00, 0x00 }, 4 } //example
};
static const int AUTHORIZED_CARDS_COUNT = sizeof(AUTHORIZED_CARDS) / sizeof(AUTHORIZED_CARDS[0]);

#endif //DRONE_IN_A_BOX_SECRETS_EXAMPLE_H
