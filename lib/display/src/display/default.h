#pragma once

#define _ST7735_SOFT  0
#define _ST7735_SPI   1
#define _ST7789_D8    2
#define _SSD1306_I2C  3

// По умолчанию

#ifndef DISPLAY_NAME
#define DISPLAY_NAME _ST7735_SOFT
#endif

#ifndef RGB_FORMAT
#define RGB_FORMAT RGB_16
#endif

#ifdef EX_X_Y
#define MAX_X     LCD_MAX_Y
#define MAX_Y     LCD_MAX_X
#else
#define MAX_X     LCD_MAX_X
#define MAX_Y     LCD_MAX_Y
#endif
