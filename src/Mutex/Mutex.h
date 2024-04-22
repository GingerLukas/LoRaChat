//
// Created by lukas on 19/04/2024.
//

#ifndef LORACHAT_MUTEX_H
#define LORACHAT_MUTEX_H


#include "Arduino.h"

struct Mutex {
    Mutex();
    explicit Mutex(bool debug) : Mutex(){
        _debug = debug;
    }
    void lock() volatile;
    void unlock() volatile;

private:
    SemaphoreHandle_t _handle;
    bool _debug = false;
};



#endif //LORACHAT_MUTEX_H
