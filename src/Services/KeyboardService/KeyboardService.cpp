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
    if(value<=0) return;

    _pending.lock();
    _pending.value.push(value);
    _pending.unlock();
}

size_t KeyboardService::available() {
    _pending.lock();
    size_t size = _pending.value.size();
    _pending.unlock();
    return size;
}

char KeyboardService::getKey() {
    char result = 0;

    _pending.lock();
    if (!_pending.value.empty()) {
        result = _pending.value.front();
        _pending.value.pop();
    }
    _pending.unlock();

    return result;
}


