#include <Arduino.h>

class RGB {
public:
  uint8_t r : 5;
  uint8_t g : 6;
  uint8_t b : 5;

public:
  RGB() {}
  RGB(uint8_t red, uint8_t green, uint8_t blue)
    : r(red >> 3), g(green >> 2), b(blue >> 3)
  {}
  RGB(uint32_t rgb) // формат 0x00rrggbb
    : r(rgb >> 19), g(rgb >> 10), b(rgb >> 3)
  {}

  // operator uint16_t() { return *(uint16_t*)this; }
  operator uint32_t() { return (uint32_t)r << 19 | (uint32_t)g << 10 | (uint32_t)b << 3; }
};
