//
// Created by lukas on 11/05/2024.
//

#ifndef LORACHAT_MESSAGEPACKET_H
#define LORACHAT_MESSAGEPACKET_H

struct MessagePacket {
    MessagePacket() = default;
    MessagePacket(float_t rssi, float_t snr, const String &str) :
            RSSI(rssi), SNR(snr), content(str) {

    }

    float_t RSSI = 0;
    float_t SNR = 0;
    String content = "";
};

#endif //LORACHAT_MESSAGEPACKET_H
