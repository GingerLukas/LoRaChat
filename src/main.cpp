#include <Arduino.h>

#include "Services/TouchScreenService/TouchScreenService.h"

#include <lvgl.h>
#include <Arduino_GFX_Library.h>
#include <SPI.h>
#include <RadioLib.h>

TouchScreenService touchScreen;

void setup() {
    // write your initialization code here
    Serial.begin(115200);

    touchScreen.begin(320,240,1);
}

void loop() {
    delay(1000);
}

