//
// Created by lukas on 19/04/2024.
//

#include "RwMutex.h"

RwMutex::RwMutex() {
    pthread_rwlock_init(&_handle, nullptr);
}

RwMutex::~RwMutex() {
    pthread_rwlock_destroy(&_handle);
}

void RwMutex::unlock() {
    pthread_rwlock_unlock(&_handle);
}

void RwMutex::lockRead() {
    while (true) {
        if (pthread_rwlock_rdlock(&_handle) == 0) return;

        esp_backtrace_print(10);

        delay(1000);
    }
}

void RwMutex::lockWrite() {
    while (true) {
        if (pthread_rwlock_wrlock(&_handle) == 0) return;

        esp_backtrace_print(10);

        delay(1000);
    }

}
