#include "draw.h"

void Draw::point(byte x, byte y)
{
  _lcd->pixel(x, y, _color.r, _color.g, _color.b);
}

void Draw::moveTo(byte x, byte y)
{
  _x = x;
  _y = y;
}

void Draw::lineTo(byte x, byte y)
{}

void Draw::line(byte x1, byte y1, byte x2, byte y2)
{}
