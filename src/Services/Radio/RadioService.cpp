//
// Created by lukas on 03/05/2024.
//

#include "RadioService.h"

volatile size_t pendingTxCounter = 0;
volatile size_t handledTxCounter = 0;

volatile size_t pendingRxCounter = 0;
volatile size_t handledRxCounter = 0;

SX1262 *irq_radio = nullptr;

void handle_dio() {
    uint16_t status = irq_radio->getIrqStatus();
    if (status & RADIOLIB_SX126X_IRQ_RX_DONE) {
        pendingRxCounter++;
    }
    else if(status & RADIOLIB_SX126X_IRQ_TX_DONE){
        pendingTxCounter++;
    }
}

RadioService::RadioService() :
        _radio(new Module(RADIO_CS, RADIO_DIO1, RADIO_RST, RADIO_BUSY)) {
    pinMode(RADIO_CS, OUTPUT);
    irq_radio = &_radio;

    //reserve max size of packet
    _tmpMessage.reserve(256);
}

void RadioService::begin() {
    ServiceBase::begin("Radio", 20, 5);
}

void RadioService::setup() {


    softAssert(_radio.begin(RADIO_FREQ) == RADIOLIB_ERR_NONE, "radio freq");

    softAssert(_radio.setBandwidth(250) == RADIOLIB_ERR_NONE, "bandwidth");
    softAssert(_radio.setSpreadingFactor(10) == RADIOLIB_ERR_NONE, "spreading factor");
    softAssert(_radio.setCodingRate(6) == RADIOLIB_ERR_NONE, "coding rate");
    softAssert(_radio.setSyncWord(0xAB) == RADIOLIB_ERR_NONE, "sync word");
    softAssert(_radio.setOutputPower(17) == RADIOLIB_ERR_NONE, "output power");
    softAssert(_radio.setCurrentLimit(140) == RADIOLIB_ERR_NONE, "current limit");
    softAssert(_radio.setPreambleLength(15) == RADIOLIB_ERR_NONE, "preamble length");
    softAssert(_radio.setCRC(2) == RADIOLIB_ERR_NONE, "crc");


    //register callback on packet
    _radio.setDio1Action(handle_dio);


    softAssert(_radio.startReceive() == RADIOLIB_ERR_NONE, "start receive");

}

void RadioService::loop() {
    handleInterrupt();

    if ((handledTxCounter == pendingTxCounter) && _txQueue.size()) {
        String txMessage = _txQueue.get();
        auto result = _radio.startTransmit(txMessage);
        if (!softAssert(result == RADIOLIB_ERR_NONE, "transmit")) {
            Serial.printf("Transmit failed with error: %d\n", result);
        }
    }
}

void RadioService::sendMessage(const String &message) {
    _txQueue.put(message);
}

size_t RadioService::available() {
    return _rxQueue.size();
}

MessagePacket RadioService::readMessage() {
    return _rxQueue.get();
}

void RadioService::handleInterrupt() {
    if(handledTxCounter != pendingTxCounter){
        handledTxCounter++;
        while (!softAssert(_radio.finishTransmit() == RADIOLIB_ERR_NONE, "start receive")) {
            delay(240);
        }
        softAssert(_radio.startReceive() == RADIOLIB_ERR_NONE, "start receive");
    }
    if (handledRxCounter != pendingRxCounter) {
        handledRxCounter++;

        size_t len = _radio.getPacketLength();
        softAssert(len <= RADIOLIB_SX126X_MAX_PACKET_LENGTH, "Invalid packet length");
        uint8_t buffer[len];
        _radio.readData(buffer, len);
        _rxQueue.put(MessagePacket(_radio.getRSSI(),
                                   _radio.getSNR(),
                                   String(buffer, len)));
    }
}


