#include <Arduino.h>

class RGB {
public:
  uint16_t data;

public:
  RGB() {}
  RGB(byte red, byte green, byte blue) : data((red >> 3) | ((green & 0xfc) << 3) | ((blue & 0xf8) << 8)) {}
  RGB(uint16_t rgb) : data((rgb >> 7) | ((rgb & 0xf0) << 3) | (rgb << 12)) {} // формат 0x0rgb

  void b(byte d) { data = ((data & 0xf800) + (d << 11)) | (data & 0x07ff); }
  void g(byte d) { data = ((data + (d << 5)) & 0x07e0) | (data & 0xf81f); }
  void r(byte d) { data = ((data + d) & 0x1f) | (data & 0xffe0); }

  operator uint16_t() { return *(uint16_t *)this; }
  operator uint32_t() { return (uint32_t)(data & 0x1f) << 19 | (uint32_t)(data & 0x7e0) << 5 | (uint32_t)(data & 0xf800) >> 8; }
};
