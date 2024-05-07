//
// Created by lukas on 07/05/2024.
//

#include "GuiService.h"

static GuiService* gui;

void handleDisplayFlush_proxy(lv_disp_drv_t *driver, const lv_area_t *area, lv_color_t *buffer)
{
    gui->handleDisplayFlush(driver, area, buffer);
}

void GuiService::handleDisplayFlush(lv_disp_drv_t *driver, const lv_area_t *area, lv_color_t *buffer) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

#if (LV_COLOR_16_SWAP != 0)
    _gfx->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#else
    _gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&buffer->full, w, h);
#endif

    lv_disp_flush_ready(driver);
}

GuiService::GuiService() {
    gui = this;
    pinMode(DISPLAY_CS, OUTPUT);
    pinMode(DISPLAY_DC, OUTPUT);
}

void GuiService::begin(uint16_t width, uint16_t height, uint8_t rotation) {
    _width = width;
    _height = height;
    _rotation = rotation;

    _bus = new Arduino_ESP32SPI(DISPLAY_DC, DISPLAY_CS, SPI_SCK, SPI_MOSI, SPI_MISO);
    _gfx = new Arduino_ST7789(_bus, GFX_NOT_DEFINED, _rotation, true);
    _canvas = new Arduino_Canvas(_width * 2, _height * 2, _gfx);

    _displayBuffer = (lv_color_t *) heap_caps_malloc(sizeof(lv_color_t) * _width * 40,
                                                  MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);

    ServiceBase::begin("GUI", 10, 10);
}

void GuiService::setup() {
#ifdef GFX_EXTRA_PRE_INIT
    GFX_EXTRA_PRE_INIT();
#endif

    // Init Display
    softAssert(_canvas->begin(80000000), "canvas begin");

    _gfx->fillScreen(BLUE);

    ledcSetup(0, 1000, 8);
    ledcAttachPin(DISPLAY_BACKLIGHT, 0);
    ledcWrite(0, 191);

    // TODO: invert?

    lv_init();

    lv_disp_draw_buf_init(&_drawBuffer, _displayBuffer, NULL, _width * 40);

    lv_disp_drv_init(&_driver);

    _driver.hor_res = _width;
    _driver.ver_res = _height;
    _driver.flush_cb = handleDisplayFlush_proxy;
    _driver.draw_buf = &_drawBuffer;
    lv_disp_drv_register(&_driver);


    //TODO input devices

    //TODO actual ui

}

void GuiService::loop() {
    lv_timer_handler();
}




