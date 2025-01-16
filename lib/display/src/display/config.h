#pragma once

// Выбор типа дисплея

// #define _ST7735_
#define _ST7789_
// #define _SSD1306_

// Выбор интерфейса

// #define LCD_SPI         // Подключает через SPI
// #define LCD_I2C         // Подключает через I2C
#define LCD_SOFT        // Программно

// Цветовая модель

// #define RGB_FORMAT RGB_M    // Монохроматический
// #define RGB_FORMAT RGB_12   // 4x4x4 bit
// #define RGB_FORMAT RGB_16   // 5x6x5 bit
#define RGB_FORMAT RGB_18   // 6x6x6 bit

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

// Подключаемые библиотеки

// #define LCD_DRAW        // Подключает класс для рисования
// #define LCD_PRINT       // Подключает класс для вывода текста


// Подключение индивидуальных настроек дисплея

#if  defined(_ST7735_)
#include <ST7735/config.h>
#elif defined(_ST7789_)
#include <ST7789/config.h>
#elif defined(_SSD1306_)
#include <SSD1306/config.h>
#endif

///////////////////////////////////////////////////////////////////////////
// Вторичные макросы, зависящие от настроек выше

#ifdef  EX_X_Y
#define MAX_X     LCD_MAX_Y
#define MAX_Y     LCD_MAX_X
#else
#define MAX_X     LCD_MAX_X
#define MAX_Y     LCD_MAX_Y
#endif
