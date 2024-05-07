//
// Created by lukas on 07/05/2024.
//

#ifndef LORACHAT_KEYBOARDSERVICE_H
#define LORACHAT_KEYBOARDSERVICE_H


#include <queue>
#include <Wire.h>
#include "Services/ServiceBase.h"
#include "Mutex/DoubleBuffer.h"

using namespace std;

class KeyboardService : public ServiceBase {
public:
    void begin();

    char getKey();
    size_t available();
protected:
    void setup() override;
    void loop() override;

    ThreadSafe<queue<char>> _pending;


};


#endif //LORACHAT_KEYBOARDSERVICE_H
