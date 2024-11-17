#include <Arduino.h>

class RGB {
public:
  uint8_t b;
  uint8_t g;
  uint8_t r;

public:
  RGB() {}
  RGB(uint8_t red, uint8_t green, uint8_t blue)
    :b(blue), g(green), r(red)
  {}
  RGB(uint32_t rgb) // формат 0x00rrggbb
    :b(rgb), g(rgb >> 8), r(rgb >> 16)
  {}

  operator uint32_t() { return *(uint32_t *)this; }
  operator uint16_t() { return ((r << 4) & 0xf00) | (g & 0xf0) | (b >> 4); }
};
