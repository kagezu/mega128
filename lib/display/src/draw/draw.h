#pragma once
#include <Arduino.h>
#include "rgb/rgb.h"


#ifndef MAX_X
#define MAX_X 0
#define MAX_Y 0
#endif


class  Draw {
protected:
  byte _fat = 2;

public:
  void fat(byte f) { _fat = f; }

  // Реализуется в данном классе
public:
  // Линии и фигуры
  void w_line(byte, byte, byte);
  void h_line(byte, byte, byte);
  void w_line_fat(byte, byte, byte);
  void h_line_fat(byte, byte, byte);

  void line(byte, byte, byte, byte);
  void round_rect(byte, byte, byte, byte, byte);
  void rect_line(byte, byte, byte, byte);
  void circle(byte, byte, byte);

  // Утолщённые линии и фигуры
  void line_fat(byte, byte, byte, byte);
  void rect_fat(byte, byte, byte, byte);
  void circle_fat(byte, byte, byte);

  // Полные фигуры
  void circle_fill(byte, byte, byte);
  void round_rect_fill(byte, byte, byte, byte, byte);

  // Реализуется в дочернем классе
public:
  virtual void pixel(byte, byte) = 0;
  virtual void rect_fill(byte, byte, byte, byte) = 0;
};

#define swap(a, b) { int16_t t = a; a = b; b = t; }

void Draw::w_line(byte x, byte y, byte x1)
{
  // if (x > x1) swap(x, x1);

  if (x > MAX_X) x = 0;
  if (x1 > MAX_X) x1 = MAX_X;
  if (x == x1) pixel(x, y);
  else  rect_fill(x, y, x1, y);
}

void Draw::h_line(byte x, byte y, byte y1)
{
  // if (y > y1) swap(y, y1);

  if (y > MAX_Y) y = 0;
  if (y1 > MAX_Y) y1 = MAX_Y;
  if (y == y1) pixel(x, y);
  else  rect_fill(x, y, x, y1);
}

void Draw::w_line_fat(byte x, byte y, byte x1)
{
  byte y1 = y + ((_fat + 1) >> 1) - 1;
  y -= _fat >> 1;
  if (y > MAX_Y) y = 0;
  if (y1 > MAX_Y) y1 = MAX_Y;
  rect_fill(x, y, x1, y1);
}

void Draw::h_line_fat(byte x, byte y, byte y1)
{
  byte x1 = x + ((_fat + 1) >> 1) - 1;
  x -= _fat >> 1;
  if (x > MAX_X) x = 0;
  if (x1 > MAX_X) x1 = MAX_X;
  rect_fill(x, y, x1, y1);
}

void Draw::line(byte x0, byte y0, byte x1, byte y1)
{
  bool steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }
  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }

  int16_t dx = x1 - x0;
  int16_t dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int8_t yStep = 1;
  if (y0 > y1) yStep = -1;

  while (x0 <= x1) {
    if (steep)
      pixel(y0, x0++);
    else
      pixel(x0++, y0);

    err -= dy;
    if (err < 0) {
      y0 += yStep;
      err += dx;
    }
  }
}
void Draw::circle(byte x, byte y, byte radius)
{
  int16_t f = 1 - radius;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * radius;
  int16_t x1 = 0;
  int16_t y1 = radius;

  pixel(x, y + radius);
  pixel(x, y - radius);
  pixel(x + radius, y);
  pixel(x - radius, y);

  while (x1 < y1) {
    if (f >= 0) {
      y1--;
      ddF_y += 2;
      f += ddF_y;
    }
    x1++;
    ddF_x += 2;
    f += ddF_x;

    pixel(x + x1, y + y1);
    pixel(x - x1, y + y1);
    pixel(x + x1, y - y1);
    pixel(x - x1, y - y1);
    pixel(x + y1, y + x1);
    pixel(x - y1, y + x1);
    pixel(x + y1, y - x1);
    pixel(x - y1, y - x1);
  }
}

void Draw::circle_fat(byte x, byte y, byte radius)
{
  int16_t f = 1 - radius;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * radius;
  int16_t dx = 0;
  int16_t dy = radius;

  byte d0 = (_fat >> 1);
  byte d1 = ((_fat + 1) >> 1) - 1;
  byte x1 = x - d0;
  byte y1 = y - d0;
  byte x2 = x + d1;
  byte y2 = y + d1;

  circle(x, y, radius - d0);
  circle(x, y, radius + d1);

  h_line(x, y1 + radius, y2 + radius);
  h_line(x, y1 - radius, y2 - radius);
  w_line(x1 + radius, y, x2 + radius);
  w_line(x1 - radius, y, x2 - radius);

  while (dx < dy) {
    if (f >= 0) {
      dy--;
      ddF_y += 2;
      f += ddF_y;
    }
    dx++;
    ddF_x += 2;
    f += ddF_x;

    h_line(x - dx, y1 - dy, y2 - dy);
    h_line(x + dx, y1 - dy, y2 - dy);
    h_line(x - dx, y1 + dy, y2 + dy);
    h_line(x + dx, y1 + dy, y2 + dy);

    w_line(x1 - dy, y - dx, x2 - dy);
    w_line(x1 - dy, y + dx, x2 - dy);
    w_line(x1 + dy, y - dx, x2 + dy);
    w_line(x1 + dy, y + dx, x2 + dy);
  }
}

void Draw::circle_fill(byte x, byte y, byte radius)
{
  radius++;
  int16_t f = 1 - radius;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * radius;
  int16_t dx = 0;
  int16_t dy = radius;

  w_line(x - radius + 1, y, x + radius - 1);
  while (dx < dy) {
    if (f >= 0) {
      dy--;
      ddF_y += 2;
      f += ddF_y;
      w_line(x - dx + 1, y + dy, x + dx - 1);
      w_line(x - dx + 1, y - dy, x + dx - 1);
    }
    dx++;
    ddF_x += 2;
    f += ddF_x;

    w_line(x - dy + 1, y + dx, x + dy - 1);
    w_line(x - dy + 1, y - dx, x + dy - 1);
  }
}

void Draw::round_rect_fill(byte x, byte y, byte width, byte height, byte radius)
{
  int16_t f = 1 - radius;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * radius;
  int16_t dx = 0;
  int16_t dy = radius;

  byte x1 = x + radius;
  byte y1 = y + radius;
  byte x2 = x + width - radius - 1;
  byte y2 = y + height - radius - 1;

  rect_fill(x, y1, x + width, y2);

  while (dx < dy) {
    if (f >= 0) {
      dy--;
      ddF_y += 2;
      f += ddF_y;

      w_line(x1 - dx, y1 - dy, x2 + dx);
      w_line(x1 - dx, y2 + dy, x2 + dx);
    }
    dx++;
    ddF_x += 2;
    f += ddF_x;

    w_line(x1 - dy, y1 - dx, x2 + dy);
    w_line(x1 - dy, y2 + dx, x2 + dy);
  }
}

void Draw::round_rect(byte x, byte y, byte width, byte height, byte radius)
{
  int16_t f = 1 - radius;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * radius;
  int16_t dx = 0;
  int16_t dy = radius;

  byte x1 = x + radius;
  byte y1 = y + radius;
  byte x2 = x + width - radius - 1;
  byte y2 = y + height - radius - 1;

  w_line(x1, y, x2);
  w_line(x1, y + height - 1, x2);
  h_line(x, y1, y2);
  h_line(x + width - 1, y1, y2);

  while (dx < dy) {
    if (f >= 0) {
      dy--;
      ddF_y += 2;
      f += ddF_y;
    }
    dx++;
    ddF_x += 2;
    f += ddF_x;

    pixel(x1 - dx, y1 - dy);
    pixel(x1 - dy, y1 - dx);
    pixel(x2 + dx, y1 - dy);
    pixel(x2 + dy, y1 - dx);
    pixel(x1 - dx, y2 + dy);
    pixel(x1 - dy, y2 + dx);
    pixel(x2 + dx, y2 + dy);
    pixel(x2 + dy, y2 + dx);
  }
}

void Draw::line_fat(byte x0, byte y0, byte x1, byte y1)
{
  bool steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }
  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }

  int16_t dx = x1 - x0;
  int16_t dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int8_t yStep = 1;
  if (y0 > y1) yStep = -1;

  y1 = y0 + ((_fat + 1) >> 1) - 1;
  y0 -= _fat >> 1;

  while (x0 <= x1) {
    if (steep)
      w_line(y0, x0++, y1);
    else
      h_line(x0++, y0, y1);

    err -= dy;
    if (err < 0) {
      y0 += yStep;
      y1 += yStep;
      err += dx;
    }
  }
}

void Draw::rect_line(byte x, byte y, byte width, byte height)
{
  w_line(x, y, x + width - 1);
  w_line(x, y + height - 1, x + width - 1);
  h_line(x, y + 1, y + height - 2);
  h_line(x + width - 1, y + 1, y + height - 2);
}

void Draw::rect_fat(byte x, byte y, byte width, byte height)
{
  byte d0 = _fat >> 1;
  byte d1 = ((_fat + 1) >> 1);
  w_line_fat(x - d0, y, x + width - 2 + d1);
  w_line_fat(x - d0, y + height - 1, x + width - 2 + d1);
  h_line_fat(x, y + d0, y + height - d1);
  h_line_fat(x + width - 1, y + d0, y + height - d1);
}
