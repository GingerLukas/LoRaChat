//
// Created by lukas on 19/04/2024.
//

#include <esp_debug_helpers.h>
#include "Mutex.h"

Mutex::Mutex() {
    _handle = xSemaphoreCreateRecursiveMutex();
}

void Mutex::lock() volatile {
    if(_debug){
        esp_backtrace_print(5);
    }
    while (true) {
        if (xSemaphoreTakeRecursive(_handle, 15 * 1000) == pdTRUE) return;

        esp_backtrace_print(10);
    }
}

void Mutex::unlock() volatile {
    if(_debug){
        esp_backtrace_print(5);
    }
    xSemaphoreGiveRecursive(_handle);
}
