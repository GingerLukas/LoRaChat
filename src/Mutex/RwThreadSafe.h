//
// Created by lukas on 19/04/2024.
//

#ifndef LORACHAT_RWTHREADSAFE_H
#define LORACHAT_RWTHREADSAFE_H

#include "RwMutex.h"

template<class T>
struct RwThreadSafe : public RwMutex{
public:
    T value;
};


#endif //LORACHAT_RWTHREADSAFE_H
