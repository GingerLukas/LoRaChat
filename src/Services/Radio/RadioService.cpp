//
// Created by lukas on 03/05/2024.
//

#include "RadioService.h"

volatile bool dioFlag = false;

void handle_dio() {
    dioFlag = true;
}

RadioService::RadioService() :
        _radio(new Module(RADIO_CS, RADIO_DIO1, RADIO_RST, RADIO_BUSY)) {
    pinMode(RADIO_CS, OUTPUT);
}

void RadioService::begin() {
    ServiceBase::begin("Radio", 10, 10);
}

void RadioService::setup() {
    _radio.lock();



    softAssert(_radio.value.begin(RADIO_FREQ) == RADIOLIB_ERR_NONE, "radio freq");

    softAssert(_radio.value.setBandwidth(250) == RADIOLIB_ERR_NONE, "bandwidth");
    softAssert(_radio.value.setSpreadingFactor(10) == RADIOLIB_ERR_NONE, "spreading factor");
    softAssert(_radio.value.setCodingRate(6) == RADIOLIB_ERR_NONE, "coding rate");
    softAssert(_radio.value.setSyncWord(0xAB) == RADIOLIB_ERR_NONE, "sync word");
    softAssert(_radio.value.setOutputPower(17) == RADIOLIB_ERR_NONE, "output power");
    softAssert(_radio.value.setCurrentLimit(140) == RADIOLIB_ERR_NONE, "current limit");
    softAssert(_radio.value.setPreambleLength(15) == RADIOLIB_ERR_NONE, "preamble length");
    softAssert(_radio.value.setCRC(false) == RADIOLIB_ERR_NONE, "crc");


    //register callback on packet
    _radio.value.setDio1Action(handle_dio);

    //softAssert(_radio.value.startReceive(), "start receive");

    _radio.unlock();
}

void RadioService::loop() {
    return;
    if (dioFlag) {
        dioFlag = false;
        String message;
        _radio.lock();
        _radio.value.readData(message);
        _radio.unlock();
        if (message.length()) {
            Serial.printf("Received: %s\n", message.c_str());
        }
    }

    softAssert(_radio.value.startReceive() == RADIOLIB_ERR_NONE, "start receive");
}

void RadioService::sendMessage(const String &message) {
    _radio.lock();
    softAssert(_radio.value.startTransmit(message.c_str()) == RADIOLIB_ERR_NONE, "transmit");
    _radio.unlock();
}


