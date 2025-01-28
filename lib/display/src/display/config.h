#pragma once
#ifndef RGB_FORMAT

// Выбор типа дисплея

#define DISPLAY_NAME    _ST7735_SOFT
// #define DISPLAY_NAME    _ST7735_SPI
// #define DISPLAY_NAME    _ST7789_D8
// #define DISPLAY_NAME    _SSD1306_I2C

// Цветовая модель

// #define RGB_FORMAT RGB_M    // Монохроматический
// #define RGB_FORMAT RGB_12   // 4x4x4 bit
#define RGB_FORMAT RGB_16   // 5x6x5 bit
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

#endif
