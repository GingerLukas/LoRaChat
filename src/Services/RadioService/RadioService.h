//
// Created by lukas on 03/05/2024.
//

#ifndef LORACHAT_RADIOSERVICE_H
#define LORACHAT_RADIOSERVICE_H


#include "Services/ServiceBase.h"
#include "modules/SX126x/SX1262.h"

class RadioService : public ServiceBase {
public:
    RadioService();

    void begin();

protected:
    void setup() override;

    void loop() override;

private:
    ThreadSafe<SX1262> _radio;
};


#endif //LORACHAT_RADIOSERVICE_H
