//
// Created by lukas on 07/05/2024.
//

#ifndef LORACHAT_GUISERVICE_H
#define LORACHAT_GUISERVICE_H

#include "Services/ServiceBase.h"
#include "lvgl.h"
#include "Arduino_GFX_Library.h"

class GuiService : public ServiceBase {
public:
    GuiService();
    void begin(uint16_t width, uint16_t height, uint8_t rotation);

    void handleDisplayFlush(lv_disp_drv_t *driver, const lv_area_t *area, lv_color_t *buffer);
protected:
    void setup() override;
    void loop() override;

private:
    uint16_t _width;
    uint16_t _height;
    uint8_t _rotation;

    Arduino_DataBus* _bus;
    Arduino_GFX* _gfx;
    Arduino_Canvas* _canvas;

    lv_color_t *_displayBuffer;
    lv_disp_draw_buf_t _drawBuffer;
    lv_disp_drv_t _driver;


};


#endif //LORACHAT_GUISERVICE_H
