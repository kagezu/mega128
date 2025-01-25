#include <Arduino.h>

class RGB {
private:
  union {
    uint16_t data;
    struct {
      byte low;
      byte high;
    };
  };

public:
  RGB() {}
  RGB(byte red, byte green, byte blue) : data((red >> 4) | (green & 0xf0) | ((blue & 0xf0) << 4)) {}
  RGB(uint32_t rgb) : data(((rgb & 0xf00000) >> 12) | ((rgb & 0xf000) >> 8) | ((byte)rgb >> 4)) {}


  void blue(byte blue) { high = blue >> 4; }
  void green(byte green) { low = (low & 0x0f) | (green & 0xf); }
  void red(byte red) { low = (low & 0xf0) | (red >> 4); }

  byte blue() { return high << 4; }
  byte green() { return  low; } //  & 0xf0; }
  byte red() { return low << 4; }


  operator uint16_t() { return *(uint16_t *)this; }
  operator uint32_t() { return (uint32_t)(data & 0x0f) << 20 | (uint32_t)(data & 0xf0) << 8 | (uint32_t)(data & 0x0f) << 4; }
};
