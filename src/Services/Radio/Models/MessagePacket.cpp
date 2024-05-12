//
// Created by lukas on 12/05/2024.
//
#include "MessagePacket.h"

void MessagePacket::convertToRaw() {
    rawData[0] = version;
    rawData[1] = username.length();
    username.getBytes(&rawData[2], username.length() + 1);
    rawData[2 + username.length()] = content.length();
    content.getBytes(&rawData[3 + username.length()], content.length() + 1);
}

void MessagePacket::parseRaw() {
    version = rawData[0];
    uint8_t usernameLen = rawData[1];
    username = String(&rawData[2], usernameLen);
    uint8_t contentLen = rawData[2 + usernameLen];
    content = String(&rawData[3 + usernameLen], contentLen);
}

uint8_t MessagePacket::getTotalLen() const {
    return 1 /*version*/ + 1 + username.length() + 1 + content.length();
}

void MessagePacket::toString(char *buffer, size_t size) const {
    snprintf(buffer, size, "[%s]: %s", username.c_str(), content.c_str());
}
