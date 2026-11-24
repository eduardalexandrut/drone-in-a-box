//
// Created by Eduard on 24/11/2026.
//

#ifndef DRONE_IN_A_BOX_ICARDREADER_H
#define DRONE_IN_A_BOX_ICARDREADER_H
#include "CardId.h"

class ICardReader {
    public:
        virtual ~ICardReader() = default;
        virtual bool readCard(CardId& cardId) = 0;
};

#endif //DRONE_IN_A_BOX_ICARDREADER_H
