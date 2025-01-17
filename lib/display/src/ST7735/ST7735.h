#pragma once
#include "display/config.h"
#include "interface/display-driver.h"

#ifdef _ST7735_

#include "rgb/rgb.h"


#ifdef LCD_SPI
#include "ST7735/ST7735_SPI.h"
#else
#include "ST7735_Soft.h"
#endif
#ifdef LCD_PRINT
#include "print/print-format.h"
#endif
#ifdef LCD_DRAW
#include "draw/draw.h"
#endif

class ST7735 : public DisplayDriver<byte>
#ifdef LCD_SPI
  , public ST7735_SPI
#else
  , public ST7735_Soft
#endif
#ifdef LCD_PRINT
  , public PrintF<byte>
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

  // void symbol(byte *, uint16_t, uint16_t, byte, byte);
  void symbol(byte *, byte, byte, byte, byte);

  void pixel(byte, byte);
  void rect_fill(byte x, byte y, byte x1, byte y1);
};

#endif
