#include <Arduino.h>

class RGB {
public:
  byte r : 5;
  byte g : 6;
  byte b : 5;

public:
  RGB() {}
  RGB(byte red, byte green, byte blue)
    :r(red >> 3), g(green >> 2), b(blue >> 3)
  {}
  RGB(uint32_t rgb) // формат 0x00rrggbb
    : r(rgb >> 3), g(rgb >> 10), b(rgb >> 19)
  {}
  RGB(uint16_t rgb) { *(uint16_t *)this = rgb; }

  operator uint16_t() { return *(uint16_t *)this; }
  operator uint32_t() { return (uint32_t)r << 19 | (uint32_t)g << 10 | (uint32_t)b << 3; }
};
