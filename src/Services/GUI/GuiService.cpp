//
// Created by lukas on 07/05/2024.
//

#include "GuiService.h"

static GuiService *gui;

void handleDisplayFlush_proxy(lv_disp_drv_t *driver, const lv_area_t *area, lv_color_t *buffer) {
    Serial.println("flush");
    gui->handleDisplayFlush(driver, area, buffer);
}

void GuiService::handleDisplayFlush(lv_disp_drv_t *driver, const lv_area_t *area, lv_color_t *buffer) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

#if (LV_COLOR_16_SWAP != 0)
    _gfx->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#else
    _gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *) &buffer->full, w, h);
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

    _gfx->fillScreen(BLACK);

    ledcSetup(0, 1000, 8);
    ledcAttachPin(DISPLAY_BACKLIGHT, 0);
    ledcWrite(0, 191);

    // TODO: invert?

    _gfx->invertDisplay(true);

    lv_init();

    lv_disp_draw_buf_init(&_drawBuffer, _displayBuffer, NULL, _width * 40);

    lv_disp_drv_init(&_driver);

    _driver.hor_res = _width;
    _driver.ver_res = _height;
    _driver.flush_cb = handleDisplayFlush_proxy;
    _driver.draw_buf = &_drawBuffer;
    lv_disp_drv_register(&_driver);


    //TODO input devices


    initTheme();

    initUI();

    lv_disp_load_scr(_screen);

    lv_timer_handler();
}

void GuiService::loop() {
    lv_timer_handler();
}

void GuiService::initTheme() {
    lv_disp_t *display = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(display, lv_palette_main(LV_PALETTE_BLUE),
                                              lv_palette_main(LV_PALETTE_RED), false,
                                              LV_FONT_DEFAULT);
    lv_disp_set_theme(display, theme);
}

void GuiService::initUI() {
    _screen = lv_obj_create(nullptr);
    lv_obj_clear_flag(_screen, LV_OBJ_FLAG_SCROLLABLE);

    _panel = lv_obj_create(_screen);
    lv_obj_set_width(_panel, 280);
    lv_obj_set_height(_panel, 200);
    lv_obj_set_align(_panel, LV_ALIGN_CENTER);
    lv_obj_clear_flag(_panel, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(_panel, lv_color_hex(0xF2F2F2), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(_panel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    _messages = lv_textarea_create(_panel);
    lv_obj_set_width(_messages, 280);
    lv_obj_set_height(_messages, 140);
    lv_obj_set_x(_messages, 0);
    lv_obj_set_y(_messages, -15);
    lv_obj_set_align(_messages, LV_ALIGN_CENTER);
    lv_textarea_set_placeholder_text(_messages, "Message history");
    lv_obj_clear_flag(_messages, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    lv_obj_set_style_text_font(_messages, LV_FONT_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);

    _input = lv_textarea_create(_panel);
    lv_obj_set_width( _input, 210);
    lv_obj_set_height( _input, LV_SIZE_CONTENT);
    lv_obj_set_x( _input, -35 );
    lv_obj_set_y( _input, 81 );
    lv_obj_set_align( _input, LV_ALIGN_CENTER );
    lv_textarea_set_placeholder_text(_input,"Type here");
    lv_textarea_set_one_line(_input,true);
    lv_obj_add_state( _input, LV_STATE_FOCUSED );
}





