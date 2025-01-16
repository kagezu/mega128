#include <Arduino.h>

class RGB {
public:
  bool data;

public:
  RGB() {}
  RGB(byte red, byte green, byte blue) : data(red | green | blue) {}
  RGB(uint16_t rgb) : data((rgb >> 8) | rgb) {}

  void b(byte d) { data = d & 1 ? data ? 0 : 1 : data; }
  void g(byte d) { data = d & 1 ? data ? 0 : 1 : data; }
  void r(byte d) { data = d & 1 ? data ? 0 : 1 : data; }

  operator uint16_t() { return data ? 0xffff : 0; }
};
