//
// Created by lukas on 03/05/2024.
//

#include "RadioService.h"


void handle_dio(){

}

RadioService::RadioService() :
        _radio(new Module(RADIO_CS, RADIO_DIO1, RADIO_RST, RADIO_BUSY)) {

}

void RadioService::begin() {
    ServiceBase::begin("Radio", 10, 10);
}

void RadioService::setup() {
    _radio.lock();

    softAssert(_radio.value.begin(RADIO_FREQ), "radio freq");
    softAssert(_radio.value.setBandwidth(250), "bandwidth");
    softAssert(_radio.value.setSpreadingFactor(10), "spreading factor");
    softAssert(_radio.value.setCodingRate(6), "coding rate");
    softAssert(_radio.value.setSyncWord(0xAB), "sync word");
    softAssert(_radio.value.setOutputPower(17), "output power");
    softAssert(_radio.value.setCurrentLimit(140), "current limit");
    softAssert(_radio.value.setPreambleLength(15), "preamble length");
    softAssert(_radio.value.setCRC(false), "crc");

    //register callback on packet
    _radio.value.setDio1Action(handle_dio);

    softAssert(_radio.value.startReceive(RADIOLIB_SX126X_RX_TIMEOUT_INF),"start receive");

    _radio.unlock();
}

void RadioService::loop() {

}


