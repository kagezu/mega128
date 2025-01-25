#include <Arduino.h>

class RGB {
private:
  byte _blue;
  byte _green;
  byte _red;

public:
  RGB() {}
  RGB(byte red, byte green, byte blue) : _blue(blue), _green(green), _red(red) {}
  RGB(uint32_t rgb) : _blue(rgb), _green(rgb >> 8), _red(rgb >> 16) {} // формат 0x00rrggbb


  void blue(byte blue) { _blue = blue; }
  void green(byte green) { _green = green; }
  void red(byte red) { _red = red; }

  byte blue() { return _blue; }
  byte green() { return _green; }
  byte red() { return _red; }


  operator uint32_t() { return *(uint32_t *)this; }

  /*
    // формат 0x0rgb
    RGB(uint16_t rgb) : b(rgb << 4), g(rgb & 0xf0), r((rgb >> 4) & 0xf0) {}
    operator uint16_t() { return ((r << 4) & 0xf00) | (g & 0xf0) | (b >> 4); }
  */
};
