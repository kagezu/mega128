#include <Arduino.h>

class RGB {
private:
  union {
    uint16_t data;
    struct {
      uint8_t low;
      uint8_t high;
    };
  };

public:
  RGB() {}
  RGB(uint8_t red, uint8_t green, uint8_t blue) : data((red >> 4) | (green & 0xf0) | ((blue & 0xf0) << 4)) {}
  RGB(uint32_t rgb) : data(((rgb & 0xf00000) >> 12) | ((rgb & 0xf000) >> 8) | ((uint8_t)rgb >> 4)) {}


  void blue(uint8_t blue) { high = blue >> 4; }
  void green(uint8_t green) { low = (low & 0x0f) | (green & 0xf); }
  void red(uint8_t red) { low = (low & 0xf0) | (red >> 4); }

  uint8_t blue() { return high << 4; }
  uint8_t green() { return  low; } //  & 0xf0; }
  uint8_t red() { return low << 4; }


  operator uint16_t() { return *(uint16_t *)this; }
  operator uint32_t() { return (uint32_t)(data & 0x0f) << 20 | (uint32_t)(data & 0xf0) << 8 | (uint32_t)(data & 0x0f) << 4; }
};
