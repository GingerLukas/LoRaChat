//
// Created by lukas on 19/04/2024.
//

#ifndef LORACHAT_SERVICEBASE_H
#define LORACHAT_SERVICEBASE_H

#include "config.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Mutex/ThreadSafe.h"
#include "Mutex/RwThreadSafe.h"

class ServiceBase {
public:
    virtual void setup() = 0;

    virtual void loop() = 0;

    [[noreturn]] static void serviceLoop(void *service);

protected:
    void begin(const char *name, uint8_t priority, uint64_t delay);

    uint64_t _loopDelay = 0;

    /// xTask
    TaskHandle_t _handle = nullptr;
    StaticTask_t _taskBuffer{};
    StackType_t _taskStack[4096]{};
};


#endif //LORACHAT_SERVICEBASE_H
