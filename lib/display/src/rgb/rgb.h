// #include "rgb/rgb/h"
#pragma once
#include <core.h>

class RGB {
public:
  union {
    uint32_t rgb;
    struct {
      uint8_t blue;
      uint8_t green;
      uint8_t red;
      uint8_t alpha;
    };
  };

public:
  RGB() {}
  RGB(uint8_t _red, uint8_t _green, uint8_t _blue) : blue(_blue), green(_green), red(_red) {}
  RGB(uint32_t _rgb) : rgb(_rgb) {}

  operator uint32_t() { return *(uint32_t *)this; }

  /*
    // формат 0x0rgb
    RGB(uint16_t rgb) : b(rgb << 4), g(rgb & 0xf0), r((rgb >> 4) & 0xf0) {}
    operator uint16_t() { return ((r << 4) & 0xf00) | (g & 0xf0) | (b >> 4); }
  */
};

















/*
#if RGB_FORMAT == RGB_M
#include "rgbm.h"
#elif RGB_FORMAT == RGB_12
#include "rgb12.h"
#elif RGB_FORMAT == RGB_16
#include "rgb16.h"
#elif RGB_FORMAT == RGB_18
#include "rgb18.h"
#endif
*/
