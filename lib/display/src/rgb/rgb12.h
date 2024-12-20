#include <Arduino.h>

class RGB {
public:
  uint16_t data;

public:
  RGB() {}
  RGB(uint8_t red, uint8_t green, uint8_t blue) : data((red >> 4) | (green & 0xf0) | ((blue & 0xf0) << 4)) {}
  RGB(uint16_t rgb) : data((rgb >> 8) | (rgb & 0xf0) | ((rgb & 0xf) << 8)) {} // формат 0x0rgb

  void b(uint8_t d) { data = ((data + (d << 8)) & 0xf00) | (data & 0xf0ff); }
  void g(uint8_t d) { data = ((data + (d << 4)) & 0xf0) | (data & 0xff0f); }
  void r(uint8_t d) { data = ((data + d) & 0xf) | (data & 0xfff0); }

  operator uint16_t() { return *(uint16_t *)this; }

  // RGB(uint32_t rgb) : data((rgb >> 20) | ((rgb & 0xf000) >> 8) | ((rgb & 0xf0) << 4)) {} // формат 0x00rrggbb
  // operator uint32_t() { return (uint32_t)(data & 0x0f) << 20 | (uint32_t)(data & 0xf0) << 8 | (uint32_t)(data & 0x0f) << 4; }
};
