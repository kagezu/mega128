#pragma once
#include <core.h>

struct Font {
  byte count_char;
  byte first_char;
  byte weight;
  byte height;
  uint16_t offset;
  uint16_t data;
};
