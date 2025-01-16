#pragma once
#include "display/config.h"

#ifdef _ST7735_
#define DISPLAY_MODEL ST7735
#include <ST7735/ST7735.h>
#elif defined(_ST7789_)
#define DISPLAY_MODEL ST7789
#include <ST7789/ST7789.h>
#elif defined(_SSD1306_)
#define DISPLAY_MODEL SSD1306
#include <SSD1306/SSD1306.h>
#endif

#define Display DISPLAY_MODEL
