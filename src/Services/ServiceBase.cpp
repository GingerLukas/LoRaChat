//
// Created by lukas on 19/04/2024.
//

#include "ServiceBase.h"

void ServiceBase::begin(const char *name, uint8_t priority, uint64_t delay) {
    _loopDelay = delay;
    _handle = xTaskCreateStatic(serviceLoop, name, 4096, this, priority, _taskStack, &_taskBuffer);
}

[[noreturn]] void ServiceBase::serviceLoop(void *servicePtr) {
    auto *service = (ServiceBase *) servicePtr;
    service->setup();

    while (true) {
        service->loop();

        delay(service->_loopDelay);
    }
}
