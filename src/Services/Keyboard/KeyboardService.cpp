//
// Created by lukas on 07/05/2024.
//

#include "KeyboardService.h"

void KeyboardService::begin() {
    ServiceBase::begin("Keyboard", 10, 5);


}

void KeyboardService::setup() {
    digitalWrite(KB_POWER_ON, HIGH);

    //wait for esp32 c3 to boot
    delay(500);

    while (1) {
        Wire.requestFrom(KB_SLAVE_ADDRESS, 1u);
        if (Wire.read() != -1) break;
        delay(10);
    }
}

void KeyboardService::loop() {
    Wire.requestFrom(KB_SLAVE_ADDRESS, 1u);
    int value = Wire.read();
    if (value <= 0) return;

    _pending.put(value);
}

size_t KeyboardService::available() {
    return _pending.size();
}

uint16_t KeyboardService::getKey() {
    return transformKeyToLvGl(_pending.get());
}

uint16_t KeyboardService::transformKeyToLvGl(uint8_t key) {
    switch (key) {
        default:
            return key;
        case 8:
            return LV_KEY_BACKSPACE;
        case 9:
            return LV_KEY_NEXT;
        case 10:
        case 13:
            return LV_KEY_ENTER;
        case 27:
            return LV_KEY_ESC;
        case 180:
            return LV_KEY_LEFT;
        case 181:
            return LV_KEY_UP;
        case 182:
            return LV_KEY_DOWN;
        case 183:
            return LV_KEY_RIGHT;
    }
}


