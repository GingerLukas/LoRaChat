//
// Created by lukas on 03/05/2024.
//

#ifndef LORACHAT_RADIOSERVICE_H
#define LORACHAT_RADIOSERVICE_H


#include "Services/ServiceBase.h"
#include "modules/SX126x/SX1262.h"
#include "Models/MessagePacket.h"

class RadioService : public ServiceBase {
public:
    RadioService();

    void begin();

    void sendMessage(const MessagePacket& message);

    size_t available();

    MessagePacket readMessage();
protected:
    void setup() override;

    void loop() override;

    void handleInterrupt();
private:
    SX1262 _radio;
    ThreadSafeQueue<MessagePacket> _rxQueue;
    ThreadSafeQueue<MessagePacket> _txQueue;
    String _tmpMessage;
};


#endif //LORACHAT_RADIOSERVICE_H
