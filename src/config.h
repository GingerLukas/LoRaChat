//
// Created by lukas on 19/04/2024.
//

#ifndef LORACHAT_CONFIG_H
#define LORACHAT_CONFIG_H

#include <cstdint>
#include "font/lv_font.h"

#define TOUCH_MODULES_GT911 // GT911 / CST_SELF / CST_MUTUAL / ZTW622 / L58 / FT3267 / FT5x06
#define TOUCH_MODULE_ADDR GT911_SLAVE_ADDRESS1 // CTS328_SLAVE_ADDRESS / L58_SLAVE_ADDRESS / CTS826_SLAVE_ADDRESS / CTS820_SLAVE_ADDRESS / CTS816S_SLAVE_ADDRESS / FT3267_SLAVE_ADDRESS / FT5x06_ADDR / GT911_SLAVE_ADDRESS1 / GT911_SLAVE_ADDRESS2 / ZTW622_SLAVE1_ADDRESS / ZTW622_SLAVE2_ADDRESS
constexpr uint8_t TOUCH_AND_KB_SCL = 8;
constexpr uint8_t TOUCH_AND_KB_SDA = 18;
constexpr uint8_t TOUCH_RES = -1;
constexpr uint8_t TOUCH_INT = 16;


constexpr uint8_t SDCARD_CS = 39;
constexpr uint8_t DISPLAY_CS = 12;
constexpr uint8_t RADIO_CS = 9;

constexpr uint8_t DISPLAY_DC = 11;
constexpr uint8_t DISPLAY_BACKLIGHT = 42;


constexpr uint8_t SPI_MOSI = 41;
constexpr uint8_t SPI_MISO = 38;
constexpr uint8_t SPI_SCK = 40;

constexpr float RADIO_FREQ = 868.0;

constexpr uint8_t RADIO_BUSY = 13;
constexpr uint8_t RADIO_RST = 17;
constexpr uint8_t RADIO_DIO1 = 45;



constexpr uint8_t KB_POWER_ON = 10;
constexpr uint8_t KB_SLAVE_ADDRESS = 0x55;


#endif //LORACHAT_CONFIG_H
