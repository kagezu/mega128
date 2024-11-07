// draw.cpp
// Реализация виртуальных методов класса Draw

#include "display.h"

#define swap(a, b) { int16_t t = a; a = b; b = t; }

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

void Display::wLine(uint8_t x, uint8_t y, uint8_t length)
{}
