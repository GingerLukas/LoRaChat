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

    uint16_t getKey();
    size_t available();
protected:
    void setup() override;
    void loop() override;

    ThreadSafe<queue<uint8_t>> _pending;

    uint16_t transformKeyToLvGl(uint8_t key);
};


#endif //LORACHAT_KEYBOARDSERVICE_H
