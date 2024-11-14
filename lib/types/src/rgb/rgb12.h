#include <Arduino.h>

class RGB {
public:
  uint8_t b : 4;
  uint8_t g : 4;
  uint8_t r : 4;

public:
  RGB() {}
  RGB(uint8_t red, uint8_t green, uint8_t blue)
    : b(blue >> 4), g(green >> 4), r(red >> 4)
  {}
  RGB(uint32_t rgb) // формат 0x00rrggbb
    :b(rgb >> 20), g(rgb >> 12), r(rgb >> 4)
  {}
  RGB(uint16_t rgb) { *(uint16_t *)this = rgb; }

  operator uint16_t() { return *(uint16_t *)this; }
  operator uint32_t() { return (uint32_t)r << 20 | (uint32_t)g << 12 | (uint32_t)b << 4; }
};
