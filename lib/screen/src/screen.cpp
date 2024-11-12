#include "screen.h"

void Screen::pixel(uint8_t x, uint8_t y, RGB color)
{
  _bitmap[y][x] = color;
}

void Screen::rectFill(uint8_t x, uint8_t y, uint8_t x1, uint8_t y1)
{
  RGB *bitmap = &_bitmap[y][x];
  uint8_t incLine = MAX_X - x1 + x;
  while (y++ <= y1) {
    while (x++ <= x1)
      *bitmap++ = _color;
    bitmap += incLine;
  }
}

void Screen::clear(RGB color)
{
  RGB *bitmap = (RGB *)_bitmap[0];
  uint16_t length = (MAX_X + 1) * (MAX_Y + 1);
  while (length--) *bitmap++ = color;
}

void Screen::copyBitmap(uint8_t x, uint8_t y, uint8_t width, uint8_t height, RGB *source)
{
  RGB *bitmap = &_bitmap[y][x];
  uint8_t inc = MAX_X - width + 1;
  for (byte j = 0; j < height; j++) {
    for (byte i = 0; i < width; i++)
      *bitmap++ = *source++;
    bitmap += inc;
  }
}

void Screen::saveBitmap(uint8_t x, uint8_t y, uint8_t width, uint8_t height, RGB *target)
{
  RGB *bitmap = &_bitmap[y][x];
  uint8_t inc = MAX_X - width + 1;
  for (byte j = 0; j < height; j++) {
    for (byte i = 0; i < width; i++)
      *target++ = *bitmap++;
    bitmap += inc;
  }
}
