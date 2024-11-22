#include <Arduino.h>

class RGB {
public:
  uint16_t data;

public:
  RGB() {}
  RGB(uint8_t red, uint8_t green, uint8_t blue) : data((red >> 3) | ((green & 0xfc) << 3) | ((blue & 0xf8) << 8)) {}
  RGB(uint32_t rgb) : data((rgb >> 19) | ((rgb & 0xf800) >> 5) | ((rgb & 0xf8) << 8)) {} // формат 0x00rrggbb

  operator uint16_t() { return *(uint16_t *)this; }
  operator uint32_t() { return (uint32_t)(data & 0x1f) << 19 | (uint32_t)(data & 0x7e0) << 5 | (uint32_t)(data & 0xf800) >> 8; }
};
