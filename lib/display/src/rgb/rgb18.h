#include <Arduino.h>

class RGB {
public:
  byte blue;
  byte green;
  byte red;

public:
  RGB() {}
  RGB(byte red, byte green, byte blue) : blue(blue), green(green), red(red) {}
  RGB(uint32_t rgb) : blue(rgb), green(rgb >> 8), red(rgb >> 16) {} // формат 0x00rrggbb

  void b(byte d) { blue += d << 2; }
  void g(byte d) { green += d << 2; }
  void r(byte d) { red += d << 2; }

  operator uint32_t() { return *(uint32_t *)this; }

  /*
    // формат 0x0rgb
    RGB(uint16_t rgb) : b(rgb << 4), g(rgb & 0xf0), r((rgb >> 4) & 0xf0) {}
    operator uint16_t() { return ((r << 4) & 0xf00) | (g & 0xf0) | (b >> 4); }
  */
};
