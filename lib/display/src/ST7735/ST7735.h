#pragma once
#include "display/config.h"

#ifdef _ST7735_

#include "rgb/rgb.h"


#ifdef LCD_SPI
#include "ST7735/ST7735_SPI.h"
#else
#include "ST7735_Soft.h"
#endif
#ifdef LCD_PRINT
#include "print/print-font.h"
#endif
#ifdef LCD_DRAW
#include "draw/draw.h"
#endif

class ST7735
#ifdef LCD_SPI
  : public ST7735_SPI
#else
  : public ST7735_Soft
#endif
#ifdef LCD_PRINT
  , public PrintFont
#endif
#ifdef LCD_DRAW
  , public Draw
#endif
{
public:
ST7735();

private:
  RGB _color = RGB(255, 255, 255);
  RGB _background = 0;

public:
  void color(RGB c) { _color = c; }
  void background(RGB b) { _background = b; }
  void clear() { rect(0, 0, LCD_MAX_X, LCD_MAX_Y, _background); }
  void clear(RGB color) { rect(0, 0, LCD_MAX_X, LCD_MAX_Y, color); }

  // Скринсейвер
  void demo(byte);
  void test(byte);

  // Реализация интерфейса PrintLCD
  void symbol(byte *source, byte x, byte y, byte dx, byte dy);

  // Реализация интерфейса GFX
  void pixel(byte, byte);
  void rect_fill(uint8_t x, uint8_t y, uint8_t x1, uint8_t y1);
};

#endif
