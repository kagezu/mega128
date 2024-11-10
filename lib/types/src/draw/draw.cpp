#include "draw.h"

#define swap(a, b) { int16_t t = a; a = b; b = t; }

void Draw::wLine(uint8_t x, uint8_t y, uint8_t x1)
{
  if (x > MAX_X) x = 0;
  if (x1 > MAX_X) x1 = MAX_X;
  if (x == x1) pixel(x, y, _color);
  else  rectFill(x, y, x1, y);
}

void Draw::hLine(uint8_t x, uint8_t y, uint8_t y1)
{
  if (y > MAX_Y) y = 0;
  if (y1 > MAX_Y) y1 = MAX_Y;
  if (y == y1) pixel(x, y, _color);
  else  rectFill(x, y, x, y1);
}

void Draw::wLineFat(uint8_t x, uint8_t y, uint8_t x1)
{
  uint8_t y1 = y + ((_fat + 1) >> 1) - 1;
  y -= _fat >> 1;
  if (y > MAX_Y) y = 0;
  if (y1 > MAX_Y) y1 = MAX_Y;
  rectFill(x, y, x1, y1);
}

void Draw::hLineFat(uint8_t x, uint8_t y, uint8_t y1)
{
  uint8_t x1 = x + ((_fat + 1) >> 1) - 1;
  x -= _fat >> 1;
  if (x > MAX_X) x = 0;
  if (x1 > MAX_X) x1 = MAX_X;
  rectFill(x, y, x1, y1);
}

void Draw::line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
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
void Draw::circle(uint8_t x, uint8_t y, uint8_t radius)
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

void Draw::circleFat(uint8_t x, uint8_t y, uint8_t radius)
{
  int16_t f = 1 - radius;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * radius;
  int16_t dx = 0;
  int16_t dy = radius;

  uint8_t d0 = (_fat >> 1);
  uint8_t d1 = ((_fat + 1) >> 1) - 1;
  uint8_t x1 = x - d0;
  uint8_t y1 = y - d0;
  uint8_t x2 = x + d1;
  uint8_t y2 = y + d1;

  circle(x, y, radius - d0);
  circle(x, y, radius + d1);

  hLine(x, y1 + radius, y2 + radius);
  hLine(x, y1 - radius, y2 - radius);
  wLine(x1 + radius, y, x2 + radius);
  wLine(x1 - radius, y, x2 - radius);

  while (dx < dy) {
    if (f >= 0) {
      dy--;
      ddF_y += 2;
      f += ddF_y;
    }
    dx++;
    ddF_x += 2;
    f += ddF_x;

    hLine(x - dx, y1 - dy, y2 - dy);
    hLine(x + dx, y1 - dy, y2 - dy);
    hLine(x - dx, y1 + dy, y2 + dy);
    hLine(x + dx, y1 + dy, y2 + dy);

    wLine(x1 - dy, y - dx, x2 - dy);
    wLine(x1 - dy, y + dx, x2 - dy);
    wLine(x1 + dy, y - dx, x2 + dy);
    wLine(x1 + dy, y + dx, x2 + dy);
  }
}

void Draw::circleFill(uint8_t x, uint8_t y, uint8_t radius)
{
  radius++;
  int16_t f = 1 - radius;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * radius;
  int16_t dx = 0;
  int16_t dy = radius;

  wLine(x - radius + 1, y, x + radius - 1);
  while (dx < dy) {
    if (f >= 0) {
      dy--;
      ddF_y += 2;
      f += ddF_y;
      wLine(x - dx + 1, y + dy, x + dx - 1);
      wLine(x - dx + 1, y - dy, x + dx - 1);
    }
    dx++;
    ddF_x += 2;
    f += ddF_x;

    wLine(x - dy + 1, y + dx, x + dy - 1);
    wLine(x - dy + 1, y - dx, x + dy - 1);
  }
}

void Draw::roundRectFill(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t radius)
{
  int16_t f = 1 - radius;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * radius;
  int16_t dx = 0;
  int16_t dy = radius;

  uint8_t x1 = x + radius;
  uint8_t y1 = y + radius;
  uint8_t x2 = x + width - radius - 1;
  uint8_t y2 = y + height - radius - 1;

  rectFill(x, y1, width, height + ddF_y);

  while (dx < dy) {
    if (f >= 0) {
      dy--;
      ddF_y += 2;
      f += ddF_y;

      wLine(x1 - dx, y1 - dy, x2 + dx);
      wLine(x1 - dx, y2 + dy, x2 + dx);
    }
    dx++;
    ddF_x += 2;
    f += ddF_x;

    wLine(x1 - dy, y1 - dx, x2 + dy);
    wLine(x1 - dy, y2 + dx, x2 + dy);
  }
}

void Draw::roundRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t radius)
{
  int16_t f = 1 - radius;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * radius;
  int16_t dx = 0;
  int16_t dy = radius;

  uint8_t x1 = x + radius;
  uint8_t y1 = y + radius;
  uint8_t x2 = x + width - radius - 1;
  uint8_t y2 = y + height - radius - 1;

  wLine(x1, y, x2);
  wLine(x1, y + height - 1, x2);
  hLine(x, y1, y2);
  hLine(x + width - 1, y1, y2);

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

void Draw::lineFat(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
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
      wLine(y0, x0++, y1);
    else
      hLine(x0++, y0, y1);

    err -= dy;
    if (err < 0) {
      y0 += yStep;
      y1 += yStep;
      err += dx;
    }
  }
}

void Draw::rectLine(uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
  wLine(x, y, x + width - 1);
  wLine(x, y + height - 1, x + width - 1);
  hLine(x, y + 1, y + height - 2);
  hLine(x + width - 1, y + 1, y + height - 2);
}

void Draw::rectFat(uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
  uint8_t d0 = _fat >> 1;
  uint8_t d1 = ((_fat + 1) >> 1);
  wLineFat(x - d0, y, x + width - 2 + d1);
  wLineFat(x - d0, y + height - 1, x + width - 2 + d1);
  hLineFat(x, y + d0, y + height - d1);
  hLineFat(x + width - 1, y + d0, y + height - d1);
}
