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
  RGB(byte red, byte green, byte blue) : data((red >> 3) | ((green & 0xfc) << 3) | ((blue & 0xf8) << 8)) {}
  RGB(uint32_t rgb) : data(((rgb & 0xf80000) >> 8) | ((rgb & 0xfc00) >> 5) | ((byte)rgb >> 3)) {}


  void blue(byte blue) { high = (high & 0x07) | (blue & 0xf8); }
  void green(byte green) { data = (data & 0x07e0) | ((green & 0xfc) << 3); }
  void red(byte red) { low = (low & 0xe0) | (red >> 3); }

  byte blue() { return high; } // & 0xf8; }
  byte green() { return  (data >> 3); } //  & 0xfc; }
  byte red() { return low << 3; }


  operator uint16_t() { return *(uint16_t *)this; }
  operator uint32_t() { return (uint32_t)(data & 0x1f) << 19 | (uint32_t)(data & 0x7e0) << 5 | (uint32_t)(data & 0xf800) >> 8; }
};
