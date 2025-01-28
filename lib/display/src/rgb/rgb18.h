#include <Arduino.h>

class RGB {
private:
  uint8_t _blue;
  uint8_t _green;
  uint8_t _red;

public:
  RGB() {}
  RGB(uint8_t red, uint8_t green, uint8_t blue) : _blue(blue), _green(green), _red(red) {}
  RGB(uint32_t rgb) : _blue(rgb), _green(rgb >> 8), _red(rgb >> 16) {} // формат 0x00rrggbb


  void blue(uint8_t blue) { _blue = blue; }
  void green(uint8_t green) { _green = green; }
  void red(uint8_t red) { _red = red; }

  uint8_t blue() { return _blue; }
  uint8_t green() { return _green; }
  uint8_t red() { return _red; }


  operator uint32_t() { return *(uint32_t *)this; }

  /*
    // формат 0x0rgb
    RGB(uint16_t rgb) : b(rgb << 4), g(rgb & 0xf0), r((rgb >> 4) & 0xf0) {}
    operator uint16_t() { return ((r << 4) & 0xf00) | (g & 0xf0) | (b >> 4); }
  */
};
