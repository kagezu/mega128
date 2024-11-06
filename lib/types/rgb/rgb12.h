#include <Arduino.h>

class RGB {
public:
  byte r : 4;
  byte g : 4;
  byte b : 4;

public:
  RGB() {}
  RGB(uint16_t rgb) { *(uint16_t *)this = rgb; }
  RGB(byte red, byte green, byte blue)
  {
    r = red >> 4;
    g = green >> 4;
    b = blue >> 4;
  }
  RGB(uint32_t rgb)
  {
    r = rgb >> 4;
    g = rgb >> 12;
    b = rgb >> 20;
  }

  operator uint16_t() { return *(uint16_t *)this; }
  operator uint32_t() { return (uint32_t)r << 20 | (uint32_t)g << 12 | (uint32_t)b << 4; }
};
