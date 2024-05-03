#include <Arduino.h>

#include "Services/TouchScreenService/TouchScreenService.h"
#include "Services/RadioService/RadioService.h"

#include <lvgl.h>
#include <Arduino_GFX_Library.h>
#include <SPI.h>
#include <RadioLib.h>

TouchScreenService touchScreen;

RadioService radio;

void setup() {
    // write your initialization code here
    Serial.begin(115200);

    touchScreen.begin(320,240,1);
    radio.begin();
}

void loop() {
    delay(1000);
}

