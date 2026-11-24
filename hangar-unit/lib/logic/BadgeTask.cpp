//
// Created by Eduard on 24/11/2026.
//

#include "BadgeTask.h"

BadgeTask::BadgeTask(HangarBlackBoard& bb, ICardReader& cardReader, BadgeWhiteList& whiteList):
    bb(bb),
    cardReader(cardReader),
    whiteList(whiteList),
    filter(false, READS_TO_PRESENT, READS_TO_REMOVED) {}

void BadgeTask::tick() {
    CardId card;
    const bool raw = cardReader.readCard(card);
    const bool present = filter.update(raw);

    // a card has just been presented: evaluate it once
    if (present && !cardPresent) { // The card has just been presented
        bool isAuth = whiteList.isAuthorized(card);
        if (isAuth) {
            bb.resetFromBadge().post(true);
        }
    }

    cardPresent = present;
}


