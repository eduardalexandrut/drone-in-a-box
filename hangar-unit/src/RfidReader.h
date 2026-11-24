//
// Created by Eduard on 24/11/2026.
//

#ifndef DRONE_IN_A_BOX_RFIDREADER_H
#define DRONE_IN_A_BOX_RFIDREADER_H
#include <Arduino.h>

#include <Arduino.h>
#include <Wire.h>
#include <MFRC522_I2C.h>
#include "ICardReader.h"

class RfidReader : public ICardReader {
public:
    explicit RfidReader(uint8_t i2cAddress, int resetPin):
        rfid(i2cAddress, resetPin) {}

    void begin() {
        Wire.setSDA(20);
        Wire.setSCL(21);
        Wire.begin();
        rfid.PCD_Init();
    };

    bool readCard(CardId& cardId) override {
        if (!rfid.PICC_IsNewCardPresent()) return false;
        if (!rfid.PICC_ReadCardSerial()) return false;

        const int length = rfid.uid.size < CardId::MAX_LENGTH ? rfid.uid.size : CardId::MAX_LENGTH;
        for (int i = 0; i < length; i++) {
            cardId.uid[i] = rfid.uid.uidByte[i];
        }
        cardId.length = length;
        return true;
    }

private:
    MFRC522 rfid;
};

#endif //DRONE_IN_A_BOX_RFIDREADER_H
