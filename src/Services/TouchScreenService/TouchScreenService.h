//
// Created by lukas on 19/04/2024.
//

#ifndef LORACHAT_TOUCHSCREENSERVICE_H
#define LORACHAT_TOUCHSCREENSERVICE_H

#include "../ServiceBase.h"
#include <Wire.h>
#include <TouchLib.h>

class TouchScreenService : public ServiceBase {
public:
    TouchScreenService();

    void begin(uint16_t width, uint16_t height, uint8_t rotation);

    TP_Point getLastTouch();
protected:
    void setup() override;
    void loop() override;
private:
    TouchLib _touch;


    uint16_t _width = 0;
    uint16_t _height = 0;
    uint8_t _rotation = 0;

    void createTranslationMapping();

    bool _swap_xy = false;
    int16_t _map_x1 = -1;
    int16_t _map_x2 = -1;
    int16_t _map_y1 = -1;
    int16_t _map_y2 = -1;

    int16_t _max_x = 0, _max_y = 0;
    RwThreadSafe<TP_Point> _lastTouch;

    void translate(TP_Point& point);
};


#endif //LORACHAT_TOUCHSCREENSERVICE_H
