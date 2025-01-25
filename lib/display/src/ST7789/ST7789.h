#pragma once
#include "comands.h"
// #include "display/config.h"
#include "rgb/rgb.h"

#ifdef _ST7789_

#ifdef LCD_PRINT
#include "print/print-format.h"
#endif
#ifdef LCD_DRAW
#include "draw/draw.h"
#endif

class ST7789 : public DisplayDriver<uint16_t>
#ifdef LCD_PRINT
  , public PrintF<uint16_t>
#endif
#ifdef LCD_DRAW
  , public Draw
#endif
{
public:
  ST7789();

private:
  RGB _color = RGB(255, 255, 255);
  RGB _background = 0;

public:
  void color(RGB c) { _color = c; }
  void background(RGB b) { _background = b; }
  void clear() { rect(0, 0, LCD_MAX_X, LCD_MAX_Y, _background); }
  void clear(RGB color) { rect(0, 0, LCD_MAX_X, LCD_MAX_Y, color); }
  void bitmap(byte *, uint16_t, uint16_t, uint16_t, uint16_t);

  // Скринсейвер
  void demo(byte);

  // Реализация интерфейса PrintF
  void symbol(byte *, uint16_t, uint16_t, byte, byte);

  // Реализация интерфейса GFX
  // void pixel(uint16_t, uint16_t);
  // void rect_fill(uint16_t x, uint16_t y, uint16_t x1, uint16_t y1);

protected:
  void send_command(byte data);
  void set_addr(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
  void send_byte(byte data);
  void send_word(uint16_t data);
  void send_rgb(byte r, byte g, byte b);
  // void send_rgb(uint16_t data); // формат 0x0rgb / RGB_16
  // void send_rgb(uint32_t color);
  void send_rgb(RGB color);
  void rect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB color);
};

#endif
