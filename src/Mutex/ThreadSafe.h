//
// Created by lukas on 19/04/2024.
//

#ifndef LORACHAT_THREADSAFE_H
#define LORACHAT_THREADSAFE_H

#include "Mutex.h"

template<class T>
struct ThreadSafe : public Mutex {
    ThreadSafe() : ThreadSafe(false){};
    explicit ThreadSafe(bool debug) : Mutex(debug){

    }
    T value;
};


#endif //LORACHAT_THREADSAFE_H
