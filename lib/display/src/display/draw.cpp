// draw.cpp
// Реализация виртуальных методов класса Draw

#include "display.h"

void Display::pixel(byte x, byte y, RGB color)
{
  if (x > MAX_X || y > MAX_Y) return;
  setAddr(x, y, x, y);

#if RGB_FORMAT == RGB_12
  sendRGB((word)0);
  sendRGB(color);
#elif RGB_FORMAT == RGB_16
  sendZero();
  sendZero();
  sendRGB(color);
#elif RGB_FORMAT == RGB_18
  sendZero();
  sendZero();
  sendZero();
  sendRGB(color);
#endif
  DISPLAY_DISCONNECT;
}

void Display::wLine(uint8_t x, uint8_t y, uint8_t x1)
{
  if (x > MAX_X) x = 0;
  if (x1 > MAX_X) x1 = MAX_X;
  if (x == x1) pixel(x, y, _color);
  else  rect(x, y, x1, y, _color);
}

void Display::hLine(uint8_t x, uint8_t y, uint8_t y1)
{
  if (y > MAX_Y) y = 0;
  if (y1 > MAX_Y) y1 = MAX_Y;
  if (y == y1) pixel(x, y, _color);
  else  rect(x, y, x, y1, _color);
}

void Display::wLineFat(uint8_t x, uint8_t y, uint8_t x1)
{
  uint8_t y1 = y + ((_fat + 1) >> 1) - 1;
  y -= _fat >> 1;
  if (y > MAX_Y) y = 0;
  if (y1 > MAX_Y) y1 = MAX_Y;
  if (x == x1) pixel(x, y, _color);
  else  rect(x, y, x1, y1, _color);
}

void Display::hLineFat(uint8_t x, uint8_t y, uint8_t y1)
{
  uint8_t x1 = x + ((_fat + 1) >> 1) - 1;
  x -= _fat >> 1;
  if (x > MAX_X) x = 0;
  if (x1 > MAX_X) x1 = MAX_X;
  if (y == y1) pixel(x, y, _color);
  else  rect(x, y, x1, y1, _color);
}

void Display::copyBitmap(uint8_t x, uint8_t y, uint8_t width, uint8_t height, RGB *source, uint16_t incLine)
{
  setAddr(x, y, x + width - 1, y + height - 1);
  for (byte j = 0; j < height; j++) {
    for (byte i = 0; i < width; i++)
      sendRGB(*source++);
    source += incLine;
  }
  DISPLAY_DISCONNECT;
}
