//
// Created by lukas on 11/05/2024.
//

#ifndef LORACHAT_MESSAGEPACKET_H
#define LORACHAT_MESSAGEPACKET_H


#include "modules/SX126x/SX126x.h"

constexpr uint8_t MESSAGE_MAX_LEN = RADIOLIB_SX126X_MAX_PACKET_LENGTH
                                    - 1 /*version*/
                                    - 1 /*username len*/
                                    - 1 /*content len*/;

struct MessagePacket {
    MessagePacket() = default;

    MessagePacket(float_t rssi, float_t snr) :
            RSSI(rssi), SNR(snr) {

    }

    MessagePacket(const String &user, const String &cont) : username(user), content(cont) {
        convertToRaw();
    }

    void toString(char* buffer, size_t size) const;

    void parseRaw();

    void convertToRaw();
    float_t RSSI = 0;
    float_t SNR = 0;

    uint8_t getTotalLen() const;
    uint8_t rawData[RADIOLIB_SX126X_MAX_PACKET_LENGTH];
    uint8_t version = 1;

    String username = "";
    String content = "";
};

#endif //LORACHAT_MESSAGEPACKET_H
