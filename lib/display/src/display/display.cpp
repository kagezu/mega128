#include "display.h"

void Display::pixel(byte x, byte y, byte r, byte g, byte b)
{
  if (x > MAX_X || y > MAX_Y) return;
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

void Display::copyBuffer(RGB *source)
{
  uint16_t length = WEIGHT * HEIGHT;
  setAddr(0, 0, MAX_X, MAX_Y);
  while (length--) sendRGB(*source++);
  DISPLAY_DISCONNECT
}

void Display::symbol(const byte *font, byte symbol, byte x, byte y, byte dx, byte dy)
{
  // setAddr(x, y, x + dx - 1, y + dy - 1);
  setAddr(x, y, x + dx - 1, y + dy);
  for (char j = 0; j < dy; j++) {
    byte data = pgm_read_byte(font + symbol * 6 - 192 + j);
    for (byte i = 0; i < dx; i++)
      if (data & (1 << i))
        sendRGB(_color);
      else
        sendRGB(_background);
  }
  for (byte i = 0; i < dx; i++)
    sendRGB(_background);

  DISPLAY_DISCONNECT
}

// тестирование дисплея

#define VIEWPORT_OFFSET 30
void Display::demo(byte d)
{
  setAddr(0, 0, MAX_X, MAX_Y);
  for (byte y = VIEWPORT_OFFSET; y < HEIGHT + VIEWPORT_OFFSET; y++) {
    word yy = y * y;

    for (byte x = VIEWPORT_OFFSET; x < WEIGHT + VIEWPORT_OFFSET; x++) {
      word xx = x * x;

      byte e = d << 2;
      word r = ((xx + yy) >> 6) + e;
      word g = ((yy - xx) >> 6) + e;
      word b = ((x * y) >> 6) - e;

      // sendRGB(RGB(r, g, b));
      sendRGB(r, g, b);
    }
  }
  DISPLAY_DISCONNECT
}
