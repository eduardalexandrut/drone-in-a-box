//
// Created by Eduard on 24/11/2026.
//

#ifndef DRONE_IN_A_BOX_UIDPRINTTASK_H
#define DRONE_IN_A_BOX_UIDPRINTTASK_H
#include "AbstractTask.h"
#include "CardId.h"
#include <Arduino.h>
#include "ICardReader.h"

class UidPrintTask : public AbstractTask {
public:
    // rawMode = true: prints 1 or 0 at every tick, to see how the reader behaves
    explicit UidPrintTask(ICardReader& reader, bool rawMode = false)
        : reader(reader), rawMode(rawMode) {}

    void tick() override {
        CardId card;
        const bool found = reader.readCard(card);

        if (rawMode) {
            Serial.print(found ? '1' : '0');
            if (++column >= 40) {       // new line every 40 readings
                Serial.println();
                column = 0;
            }
            return;
        }

        if (found && !(card == lastPrinted)) {
            printForSecrets(card);
            lastPrinted = card;
        }
    }

private:
    // prints the UID in the format used in secrets.h
    static void printForSecrets(const CardId& card) {
        Serial.print("{ { ");
        for (int i = 0; i < card.length; i++) {
            Serial.print("0x");
            if (card.uid[i] < 0x10) Serial.print('0');
            Serial.print(card.uid[i], HEX);
            if (i < card.length - 1) Serial.print(", ");
        }
        Serial.print(" }, ");
        Serial.print(card.length);
        Serial.println(" }");
    }

    ICardReader& reader;
    bool rawMode;
    CardId lastPrinted;
    int column = 0;
};
#endif //DRONE_IN_A_BOX_UIDPRINTTASK_H
