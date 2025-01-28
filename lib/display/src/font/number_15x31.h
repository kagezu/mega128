#pragma once
#include "font.h"

static const uint8_t data_number_15x31[] PROGMEM = {
  // char '+'
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xfc, 0xfc, 0xfc, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  // char ','
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcf, 0xcf, 0x3f, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00,

  // char '-'
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  // char '.'
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00,

  // char '/'
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xf0, 0xff, 0xff, 0x0f,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xf0, 0xff, 0xff, 0x0f, 0x0f, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xf0, 0xf0, 0xff, 0xff, 0x0f, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x0f, 0x0f, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  // char '0'
  0xf0, 0xf0, 0xfc, 0xfc, 0x0f, 0x0f, 0x03, 0x03, 0x03, 0xcf, 0xcf, 0xfc, 0xfc, 0xf0, 0xf0,
  0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xc0, 0xc0, 0x3c, 0x3f, 0x03, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x3c, 0x3c, 0x03, 0x03, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
  0x00, 0x00, 0x03, 0x03, 0x0f, 0x0f, 0x0c, 0x0c, 0x0c, 0x0f, 0x0f, 0x03, 0x03, 0x00, 0x00,

  // char '1'
  0x30, 0x30, 0x30, 0x30, 0x3c, 0x3c, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0f, 0x0f, 0x0f, 0x0f, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,

  // char '2'
  0xf0, 0xf0, 0xfc, 0xfc, 0x0f, 0x0f, 0x03, 0x03, 0x03, 0x03, 0x0f, 0xff, 0xfc, 0xfc, 0xf0,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xc0, 0xf0, 0xff, 0x3f, 0x3f, 0x0f,
  0xc0, 0xc0, 0xf0, 0xf0, 0x3c, 0x3c, 0x0f, 0x0f, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x0f, 0x0f, 0x0f, 0x0f, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,

  // char '3'
  0xf0, 0xf0, 0xfc, 0xfc, 0x0f, 0x0f, 0x03, 0x03, 0x03, 0x03, 0x0f, 0x1f, 0xfc, 0xfc, 0xf0,
  0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0xff, 0xdf, 0x8f,
  0xf0, 0xf0, 0xf0, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xff, 0xff, 0xff,
  0x00, 0x00, 0x03, 0x03, 0x0f, 0x0f, 0x0c, 0x0c, 0x0c, 0x0c, 0x0f, 0x0f, 0x03, 0x03, 0x00,

  // char '4'
  0x00, 0x00, 0x00, 0x00, 0xf0, 0xf0, 0xff, 0xff, 0x0f, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xfc, 0xfc, 0xff, 0xff, 0x03, 0x03, 0xc0, 0xc0, 0xc0, 0xc0, 0x00, 0x00, 0x00,
  0xff, 0xff, 0xff, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xff, 0xff, 0xff, 0xff, 0xc0, 0xc0, 0xc0,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0x00, 0x00,

  // char '5'
  0xff, 0xff, 0xff, 0xff, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
  0x0f, 0x0f, 0x0f, 0x0f, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x3c, 0xfc, 0xf0, 0xf0, 0xc0,
  0xf0, 0xf0, 0xf0, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
  0x00, 0x00, 0x03, 0x03, 0x0f, 0x0f, 0x0c, 0x0c, 0x0c, 0x0c, 0x0f, 0x0f, 0x03, 0x03, 0x00,

  // char '6'
  0x00, 0x00, 0xc0, 0xc0, 0xf0, 0xf0, 0x3c, 0x3c, 0x0f, 0x0f, 0x03, 0x03, 0x03, 0x03, 0x03,
  0xfc, 0xfc, 0xff, 0xff, 0x33, 0x33, 0x30, 0x30, 0x30, 0x30, 0xf0, 0xf0, 0xc0, 0xc0, 0x00,
  0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
  0x00, 0x00, 0x03, 0x03, 0x0f, 0x0f, 0x0c, 0x0c, 0x0c, 0x0c, 0x0f, 0x0f, 0x03, 0x03, 0x00,

  // char '7'
  0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0xff, 0xff, 0xff, 0xff,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xc0, 0xff, 0xff, 0x3f, 0x3f, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xf0, 0xff, 0xff, 0x0f, 0x0f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  // char '8'
  0xf0, 0xf0, 0xfc, 0xfc, 0x0f, 0x0f, 0x03, 0x03, 0x03, 0x0f, 0x0f, 0xfc, 0xfc, 0xf0, 0xf0,
  0x03, 0x03, 0xcf, 0xcf, 0xfc, 0xfc, 0x30, 0x30, 0x30, 0xfc, 0xfc, 0xcf, 0xcf, 0x03, 0x03,
  0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
  0x00, 0x00, 0x03, 0x03, 0x0f, 0x0f, 0x0c, 0x0c, 0x0c, 0x0f, 0x0f, 0x03, 0x03, 0x00, 0x00,

  // char '9'
  0xe0, 0xf0, 0xfc, 0xfc, 0x0f, 0x0f, 0x03, 0x03, 0x03, 0x0f, 0x0f, 0xfc, 0xfc, 0xf0, 0xe0,
  0x07, 0x0f, 0x3f, 0x3f, 0xf0, 0xf0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xff, 0xff, 0xff, 0xff,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xc0, 0xfc, 0xfc, 0x3f, 0x3f, 0x03,
  0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0f, 0x0f, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,

  // char ':'
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x3c, 0x3c, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00,

};

static const Font number_15x31 PROGMEM = {
16,                          // count_char
48,                          // first_char
15,                          // weight
31,                          // height
0,                           // Моношрифт
(uint16_t)data_number_15x31  // font data
};
