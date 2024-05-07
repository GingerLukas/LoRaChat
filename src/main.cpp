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

void setup() {
    // write your initialization code here
    Serial.begin(115200);

    Wire.begin(TOUCH_AND_KB_SDA, TOUCH_AND_KB_SCL);

    keyboard.begin();
    touchScreen.begin(width, height, rotation);
    radio.begin();
    gui.begin(width, height, rotation);
}

TouchPoint last;

void loop() {
    TouchPoint current = touchScreen.getLastTouch();
    if (current != last) {
        Serial.printf("X: %d, Y: %d, age: %d ms\n", current.x, current.y, current.age());
        last = current;
        radio.sendMessage("Touched!");
    }

    while (keyboard.available()) {
        Serial.println(keyboard.getKey());
    }

    delay(20);
}

