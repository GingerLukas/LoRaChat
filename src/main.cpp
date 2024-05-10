#include <Arduino.h>

#include "Services/TouchScreen/TouchScreenService.h"
#include "Services/Radio/RadioService.h"
#include "Services/Keyboard/KeyboardService.h"
#include "Services/GUI/GuiService.h"

#include <lvgl.h>
#include <Arduino_GFX_Library.h>
#include <SPI.h>
#include <RadioLib.h>

constexpr uint16_t width = 320;
constexpr uint16_t height = 240;
constexpr uint8_t rotation = 1;

TouchScreenService touchScreen;

RadioService radio;

KeyboardService keyboard;

GuiService gui;

uint16_t kbCallback(){
    return keyboard.getKey();
}

TouchPoint touchCallback(){
    return touchScreen.getLastTouch();
}

void setup() {
    // write your initialization code here
    Serial.begin(115200);

    gui.registerKeyBoardCallback(kbCallback);
    gui.registerTouchCallback(touchCallback);

    Wire.begin(TOUCH_AND_KB_SDA, TOUCH_AND_KB_SCL);

    keyboard.begin();
    touchScreen.begin(width, height, rotation);
    radio.begin();
    gui.begin(width, height, rotation);
}

TouchPoint last;

void loop() {
    delay(1000);
}

