#pragma once
#ifndef RGB_FORMAT

// Выбор типа дисплея

#define _ST7735_
// #define _ST7789_
// #define _SSD1306_

// Выбор интерфейса

// #define LCD_SPI         // Подключает через SPI
// #define LCD_I2C         // Подключает через I2C
#define LCD_SOFT        // Программно

// Цветовая модель

// #define RGB_FORMAT RGB_M    // Монохроматический
#define RGB_FORMAT RGB_12   // 4x4x4 bit
// #define RGB_FORMAT RGB_16   // 5x6x5 bit
// #define RGB_FORMAT RGB_18   // 6x6x6 bit

// Повороты дисплея
//    \   |   FLIP_X  |   FLIP_Y  |   EX_X_Y  |
// ============================================
//   0°   |     -     |     -     |     -     |
//  90°   |     -     |     +     |     +     |
// 180°   |     +     |     +     |     -     |
// 270°   |     +     |     -     |     +     |

// #define FLIP_X
#define FLIP_Y
#define EX_X_Y

// Interface pixel format
#define RGB_12 0x03 // 4x4x4 bit
#define RGB_16 0x05 // 5x6x5 bit
#define RGB_18 0x06 // 6x6x6 bit (24 bit transfer)

#endif
