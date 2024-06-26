//
// Created by lukas on 19/04/2024.
//

#include "TouchScreenService.h"

static volatile bool touchPending = false;

void handle_Touch() {
    touchPending = true;
}

TouchScreenService::TouchScreenService() : _touch(Wire, TOUCH_AND_KB_SDA, TOUCH_AND_KB_SCL, TOUCH_MODULE_ADDR) {
    pinMode(TOUCH_INT, INPUT);
}

void TouchScreenService::begin(uint16_t width, uint16_t height, uint8_t rotation) {
    _width = width;
    _height = height;
    _rotation = rotation;

    createTranslationMapping();

    ServiceBase::begin("Touch", 10, 5);
}

void TouchScreenService::setup() {
    attachInterrupt(digitalPinToInterrupt(TOUCH_INT), handle_Touch, RISING);

    _touch.init();
}

void TouchScreenService::loop() {
    bool available = touchPending;
    touchPending = false;

    if (available && _touch.read()) {
        TouchPoint t = _touch.getPoint(0);
        translate(t);

        _lastTouch = t;
    }
}

void TouchScreenService::createTranslationMapping() {
    _max_x = _width - 1;
    _max_y = _height - 1;
    if (_map_x1 == -1) {
        switch (_rotation) {
            case 3:
                _swap_xy = true;
                _map_x1 = _max_x;
                _map_x2 = 0;
                _map_y1 = 0;
                _map_y2 = _max_y;
                break;
            case 2:
                _swap_xy = false;
                _map_x1 = _max_x;
                _map_x2 = 0;
                _map_y1 = _max_y;
                _map_y2 = 0;
                break;
            case 1:
                _swap_xy = true;
                _map_x1 = 0;
                _map_x2 = _max_x;
                _map_y1 = _max_y;
                _map_y2 = 0;
                break;
            case 0:
            default:
                _swap_xy = false;
                _map_x1 = 0;
                _map_x2 = _max_x;
                _map_y1 = 0;
                _map_y2 = _max_y;
                break;
        }
    }

}

void TouchScreenService::translate(TouchPoint &point) {
    uint16_t x, y;
    if (_swap_xy) {
        x = map(point.y, _map_x1, _map_x2, 0, _max_x);
        y = map(point.x, _map_y1, _map_y2, 0, _max_y);
    } else {
        x = map(point.x, _map_x1, _map_x2, 0, _max_x);
        y = map(point.y, _map_y1, _map_y2, 0, _max_y);
    }
    point.x = x;
    point.y = y;
}

TouchPoint TouchScreenService::getLastTouch() {
    return _lastTouch;
}
