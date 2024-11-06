#include "display.h"

#define swap(a, b) { int16_t t = a; a = b; b = t; }

void Display::pixel(byte x, byte y, byte r, byte g, byte b)
{
  if (x >= MAX_X || y >= MAX_Y) return;
  setAddr(x, y, x, y);

#if RGB_FORMAT == RGB_12
  sendRGB((word)0);
  sendRGB(r, g, b);
#elif RGB_FORMAT == RGB_16
  sendZero();
  sendZero();
  sendRGB(r, g, b);
#elif RGB_FORMAT == RGB_18
  sendZero();
  sendZero();
  sendZero();
  sendRGB(r, g, b);
#endif

  DISPLAY_DISCONNECT
}

void Display::pixel(byte x, byte y, RGB color)
{
  if (x >= MAX_X || y >= MAX_Y) return;
  setAddr(x, y, x, y);

#if RGB_FORMAT == RGB_12
  sendRGB((word)0);
  sendRGB(color);
#elif RGB_FORMAT == RGB_16
  sendZero();
  sendZero();
  sendRGB(color);
#elif RGB_FORMAT == RGB_18
  sendZero();
  sendZero();
  sendZero();
  sendRGB(color);
#endif

  DISPLAY_DISCONNECT;
}

void Display::line(byte x0, byte y0, byte x1, byte y1, RGB color)
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
  int8_t yStep;

  if (y0 < y1) yStep = 1;
  else yStep = -1;

  while (x0 <= x1) {
    if (steep)
      pixel(y0, x0++, color);
    else
      pixel(x0++, y0, color);

    err -= dy;
    if (err < 0) {
      y0 += yStep;
      err += dx;
    }
  }
}

void Display::circle(byte x, byte y, byte r, RGB color)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x1 = 0;
  int16_t y1 = r;

  pixel(x, y + r, color);
  pixel(x, y - r, color);
  pixel(x + r, y, color);
  pixel(x - r, y, color);

  while (x1 < y1) {
    if (f >= 0) {
      y1--;
      ddF_y += 2;
      f += ddF_y;
    }
    x1++;
    ddF_x += 2;
    f += ddF_x;

    pixel(x + x1, y + y1, color);
    pixel(x - x1, y + y1, color);
    pixel(x + x1, y - y1, color);
    pixel(x - x1, y - y1, color);
    pixel(x + y1, y + x1, color);
    pixel(x - y1, y + x1, color);
    pixel(x + y1, y - x1, color);
    pixel(x - y1, y - x1, color);
  }
}

void Display::symbol(const byte *font, byte symbol, byte x, byte y, byte dx, byte dy)
{
  // setAddr(x, y, x + dx - 1, y + dy - 1);
  setAddr(x, y, x + dx - 1, y + dy);
  for (char j = 0; j < dy; j++) {
    byte data = pgm_read_byte(font + symbol * 6 - 192 + j);
    for (byte i = 0; i < dx; i++)
      if (data & (1 << i))
        sendRGB(0xff, 0xff, 0xff);
      else
        sendRGB(0x00, 0x00, 0x00);
  }

  for (byte i = 0; i < dx; i++)
    sendRGB(0x00, 0x00, 0x00);

  DISPLAY_DISCONNECT
}

// тестирование дисплея

#define VIEWPORT_OFFSET 30
void Display::demo(byte d)
{
  setAddr(0, 0, MAX_X - 1, MAX_Y - 1);
  for (byte y = VIEWPORT_OFFSET; y < MAX_Y + VIEWPORT_OFFSET; y++) {
    word yy = y * y;

    for (byte x = VIEWPORT_OFFSET; x < MAX_X + VIEWPORT_OFFSET; x++) {
      word xx = x * x;

      byte e = d << 2;
      word r = ((xx + yy) >> 6) + e;
      word g = ((yy - xx) >> 6) + e;
      word b = ((x * y) >> 6) - e;

      sendRGB(r, g, b);
    }
  }
  DISPLAY_DISCONNECT
}
