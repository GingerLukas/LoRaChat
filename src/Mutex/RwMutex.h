//
// Created by lukas on 19/04/2024.
//

#ifndef LORACHAT_RWMUTEX_H
#define LORACHAT_RWMUTEX_H


#include "pthread.h"
#include <esp_debug_helpers.h>
#include <Arduino.h>

struct RwMutex {
    RwMutex();

public:
    virtual ~RwMutex();

    void lockRead();

    void lockWrite();

    void unlock();

protected:
    pthread_rwlock_t _handle = PTHREAD_RWLOCK_INITIALIZER;
};



#endif //LORACHAT_RWMUTEX_H
