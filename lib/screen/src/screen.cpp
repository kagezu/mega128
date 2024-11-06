#include "screen.h"

void Screen::point(byte x, byte y)
{
  _lcd->pixel(x, y, _color.r, _color.g, _color.b);
}

void Screen::moveTo(byte x, byte y)
{
  _x = x;
  _y = y;
}

void Screen::lineTo(byte x, byte y)
{}

void Screen::line(byte x1, byte y1, byte x2, byte y2)
{}
