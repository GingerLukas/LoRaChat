//
// Created by lukas on 07/05/2024.
//

#include "GuiService.h"

static GuiService *gui;

void handleDisplayFlush_proxy(lv_disp_drv_t *driver, const lv_area_t *area, lv_color_t *buffer) {
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

    ServiceBase::begin("GUI", 10, 5);
}

void GuiService::setup() {
    //this is from lvgl example (as the rest of this gui code), no idea if it will be used, leaving it here tho
#ifdef GFX_EXTRA_PRE_INIT
    GFX_EXTRA_PRE_INIT();
#endif

    // Init Display
    softAssert(_canvas->begin(80000000), "canvas begin");

    _gfx->fillScreen(BLACK);

    ledcSetup(0, 1000, 8);
    ledcAttachPin(DISPLAY_BACKLIGHT, 0);
    ledcWrite(0, 255);

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

    static lv_coord_t col[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row[] = {LV_GRID_FR(1),
                               LV_GRID_FR(1),
                               LV_GRID_FR(1),
                               LV_GRID_FR(1),
                               LV_GRID_FR(1),
                               LV_GRID_TEMPLATE_LAST};

    _panel = lv_obj_create(_screen);
    lv_obj_set_size(_panel, 320, 240);
    lv_obj_set_align(_panel, LV_ALIGN_CENTER);
    lv_obj_clear_flag(_panel, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(_panel, lv_color_hex(0xF2F2F2), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(_panel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_grid_dsc_array(_panel, col, row);

    _messages = lv_textarea_create(_panel);
    lv_textarea_set_placeholder_text(_messages, "Message history");
    lv_obj_clear_flag(_messages, LV_OBJ_FLAG_CLICK_FOCUSABLE);
    lv_obj_set_style_text_font(_messages, LV_FONT_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_grid_cell(_messages, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 4);

    _input = lv_textarea_create(_panel);
    lv_textarea_set_placeholder_text(_input, "Type here");
    lv_textarea_set_one_line(_input, true);
    lv_obj_add_state(_input, LV_STATE_FOCUSED);
    lv_obj_set_grid_cell(_input, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 4, 1);
}





