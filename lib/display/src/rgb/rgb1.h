#include <Arduino.h>

class RGB {
public:
  byte data;

public:
  RGB() {}
  RGB(uint8_t red, uint8_t green, uint8_t blue) : data(red | green | blue) {}
  RGB(uint16_t rgb) : data((rgb >> 8) | rgb) {}

  void b(uint8_t d) { data = d & 1 ? data ? 0 : 1 : data; }
  void g(uint8_t d) { data = d & 1 ? data ? 0 : 1 : data; }
  void r(uint8_t d) { data = d & 1 ? data ? 0 : 1 : data; }

  operator uint16_t() { return data ? 0xffff : 0; }
};
