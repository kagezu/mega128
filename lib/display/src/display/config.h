#pragma once

// #define _ST7735_
#define _SSD1306_





#if  defined(_ST7735_)
#include <ST7735/config.h>
#elif defined(_SSD1306_)
#include <SSD1306/config.h>
#endif

#ifndef INIT_SPI
#define INIT_SPI
#endif
