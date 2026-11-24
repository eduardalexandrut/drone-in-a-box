//
// Created by Eduard on 24/11/2026.
//

#ifndef DRONE_IN_A_BOX_BADGETASK_H
#define DRONE_IN_A_BOX_BADGETASK_H
#include "AbstractTask.h"
#include "BadgeWhiteList.h"
#include "ConsecutiveFilter.h"
#include "HangarBlackBoard.h"
#include "ICardReader.h"


class BadgeTask : public AbstractTask {
public:
    BadgeTask(HangarBlackBoard &bb, ICardReader &cardReader, BadgeWhiteList &whiteList);

    void tick() override;

private:
    static const int READS_TO_PRESENT = 1;
    static const int READS_TO_REMOVED = 3;

    HangarBlackBoard &bb;
    ICardReader &cardReader;
    BadgeWhiteList &whiteList;
    ConsecutiveFilter filter;

    bool cardPresent = false;
};


#endif //DRONE_IN_A_BOX_BADGETASK_H
