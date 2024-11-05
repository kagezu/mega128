#include <Arduino.h>

class RGB {
public:
  byte b;
  byte g;
  byte r;

public:
  RGB() {}
  RGB(byte red, byte green, byte blue)
  {
    r = red;
    g = green;
    b = blue;
  }
  RGB(uint32_t rgb)
  {
    byte *p = (byte *)&rgb;
    b = *p++;
    g = *p++;
    r = *p;
  }

  operator uint32_t() { return *(uint32_t *)this; }
};
