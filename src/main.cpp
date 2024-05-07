#include <Arduino.h>

#include "Services/TouchScreen/TouchScreenService.h"
#include "Services/Radio/RadioService.h"
#include "Services/Keyboard/KeyboardService.h"

#include <lvgl.h>
#include <Arduino_GFX_Library.h>
#include <SPI.h>
#include <RadioLib.h>

TouchScreenService touchScreen;

RadioService radio;

KeyboardService keyboard;

void setup() {
    // write your initialization code here
    Serial.begin(115200);

    Wire.begin(TOUCH_AND_KB_SDA, TOUCH_AND_KB_SCL);

    keyboard.begin();
    touchScreen.begin(320,240,1);
    radio.begin();
}

TouchPoint last;
void loop() {
    TouchPoint current = touchScreen.getLastTouch();
    if(current != last){
        Serial.printf("X: %d, Y: %d, age: %d ms\n",current.x, current.y, current.age());
        last = current;
        radio.sendMessage("Touched!");
    }

    while (keyboard.available()){
        Serial.println(keyboard.getKey());
    }

    delay(20);
}

