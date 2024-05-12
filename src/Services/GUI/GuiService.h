//
// Created by lukas on 07/05/2024.
//

#ifndef LORACHAT_GUISERVICE_H
#define LORACHAT_GUISERVICE_H

#include "Services/ServiceBase.h"
#include "lvgl.h"
#include "Arduino_GFX_Library.h"
#include "../TouchScreen/Models/TouchPoint.h"
#include "Services/Radio/Models/MessagePacket.h"

class GuiService : public ServiceBase {
public:
    GuiService();

    void begin(uint16_t width, uint16_t height, uint8_t rotation);

    void registerKeyBoardCallback(uint16_t (*kbCallback)());
    void registerTouchCallback(TouchPoint (*touchCallback)());

    void registerMessageSentCallback(void (*messageSentCallback)(const MessagePacket&));
    void addMessage(const MessagePacket& message);

    void handleDisplayFlush(lv_disp_drv_t *driver, const lv_area_t *area, lv_color_t *buffer);
    void handleKeyBoard(lv_indev_drv_t *driver, lv_indev_data_t *data);
    void handleTouch(lv_indev_drv_t *driver, lv_indev_data_t *data);

protected:
    void setup() override;

    void loop() override;

private:
    Mutex lvObj;
    bool enterPressed = false;
    String _user;

    uint16_t invokeKbCallback();
    uint16_t (*_kbCallback)() = nullptr;
    uint16_t _lastKey = 0;

    TouchPoint invokeTouchCallback();
    TouchPoint (*_touchCallback)() = nullptr;

    void invokeMessageSent(const MessagePacket& message);
    void (*_messageSentCallback)(const MessagePacket&);

    uint16_t _width;
    uint16_t _height;
    uint8_t _rotation;

    Arduino_DataBus *_bus = nullptr;
    Arduino_GFX *_gfx = nullptr;
    Arduino_Canvas *_canvas = nullptr;

    lv_color_t *_displayBuffer = nullptr;
    lv_disp_draw_buf_t _drawBuffer;
    lv_disp_drv_t _driver;

    lv_group_t* _inputGroup;

    lv_indev_drv_t _kbDriver;
    lv_indev_t* _kbDevice = nullptr;

    lv_indev_drv_t _touchDriver;
    lv_indev_t* _touchDevice = nullptr;

    void initTheme();

    void initChatScreen();
    void initSettingsScreen();
    enum EScreen{
        None,
        Settings,
        Chat
    };
    EScreen _currentScreen = None;
    void setScreen(EScreen screen);

    lv_obj_t *_settingScreen;
    lv_obj_t *_username;

    lv_obj_t *_chatScreen;
    lv_obj_t *_panel;
    lv_obj_t *_messages;
    lv_obj_t *_input;
};


#endif //LORACHAT_GUISERVICE_H
