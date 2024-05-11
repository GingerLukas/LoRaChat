//
// Created by lukas on 03/05/2024.
//

#include "RadioService.h"

volatile size_t pendingCounter = 0;
volatile size_t handledCounter = 0;
volatile bool transmissionInProgress = false;

void handle_dio() {
    pendingCounter++;
}

RadioService::RadioService() :
        _radio(new Module(RADIO_CS, RADIO_DIO1, RADIO_RST, RADIO_BUSY)) {
    pinMode(RADIO_CS, OUTPUT);

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

    if (!transmissionInProgress && _txQueue.size()) {
        transmissionInProgress = true;
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
    while (handledCounter != pendingCounter) {
        handledCounter++;

        if (transmissionInProgress) {
            transmissionInProgress = false;
            while (!softAssert(_radio.finishTransmit() == RADIOLIB_ERR_NONE, "start receive")) {
                delay(240);
            }
            while (!softAssert(_radio.startReceive() == RADIOLIB_ERR_NONE, "start receive")) {
                delay(240);
            }
            continue;
        }

        size_t len = _radio.getPacketLength();
        uint8_t buffer[len];
        _radio.readData(buffer, len);
        _rxQueue.put(MessagePacket(_radio.getRSSI(),
                                   _radio.getSNR(),
                                   String(buffer, len)));
    }
}


