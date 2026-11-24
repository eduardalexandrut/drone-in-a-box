//
// Created by Eduard on 24/11/2026.
//

#ifndef DRONE_IN_A_BOX_BADGEWHITELIST_H
#define DRONE_IN_A_BOX_BADGEWHITELIST_H
#include "CardId.h"

class BadgeWhiteList {
    public:
        BadgeWhiteList(const CardId* authorizedCards, int count):
            authorizedCards(authorizedCards), count(count) {}

        bool isAuthorized(const CardId& cardId) const {
            for (int i = 0; i < count; i++) {
                if (authorizedCards[i] == cardId) {
                    return true;
                }
            }
            return false;
        }

private:
    const CardId* authorizedCards;
    int count;
};

#endif //DRONE_IN_A_BOX_BADGEWHITELIST_H
