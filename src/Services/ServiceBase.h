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
protected:
    virtual void setup() = 0;

    virtual void loop() = 0;

    [[noreturn]] static void serviceLoop(void *service);

    void begin(const char *name, uint8_t priority, uint64_t delay);

    uint64_t _loopDelay = 0;

    /// xTask
    TaskHandle_t _handle = nullptr;
    StaticTask_t _taskBuffer{};
    StackType_t _taskStack[4096]{};

    void softAssert(uint32_t value, const String &text) {
        if (value == 0) {
            Serial.printf("Soft assert failed! (%u): %s\n", value, text.c_str());
        }
    }
};


#endif //LORACHAT_SERVICEBASE_H
