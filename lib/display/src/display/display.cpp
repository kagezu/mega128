#include "display.h"

// Реализация виртуальных методов класса Draw

void Display::pixel(byte x, byte y, RGB color)
{
  if (x > MAX_X || y > MAX_Y) return;

#if FLIP_X
  x = MAX_X - x;
#endif
#if FLIP_Y
  y = MAX_Y - y;
#endif
#if EX_X_Y
  setAddr(y, x, y, x);
#else
  setAddr(x, y, x, y);
#endif

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

inline void Display::rectFill(uint8_t x, uint8_t y, uint8_t x1, uint8_t y1)
{
#if FLIP_X
  uint8_t t = x;
  x = MAX_X - x1;
  x1 = MAX_X - t;
#endif
#if FLIP_Y
  uint8_t u = y;
  y = MAX_Y - y1;
  y1 = MAX_Y - u;
#endif
#if EX_X_Y
  rect(y, x, y1, x1, _color);
#else
  rect(x, y, x1, y1, _color);
#endif
}

void Display::scanBitmap(uint8_t x, uint8_t y, uint8_t width, uint8_t height, RGB *source)
{
  setAddr(x, y, x + width - 1, y + height - 1);
  uint16_t length = width * height;
  while (length--)
    sendRGB(*source++);

  DISPLAY_DISCONNECT;
}

void Display::scanBitmap(RGB *source)
{
  uint16_t length = (LCD_MAX_X + 1) * (LCD_MAX_Y + 1);
  setAddr(0, 0, LCD_MAX_X, LCD_MAX_Y);
  while (length--) sendRGB(*source++);

  // #if !(FLIP_X || FLIP_Y || EX_X_Y)
  //   while (length--) sendRGB(*source++);
  // #endif

  DISPLAY_DISCONNECT;
}

void Display::symbol(byte *source, byte x, byte y, byte dx, byte dy)
{
  setAddr(x, y, x + dx - 1, y + dy - 1);
  // setAddr(x, y, x + dx - 1, y + dy);

  for (byte j = 0; j < dy; j++) {
    for (byte i = 0; i < dx; i++) {
      byte data = pgm_read_byte(source + i);
      if (data & (1u << j))
        sendRGB(_color);
      else
        sendRGB(_background);
    }
  }
  // for (byte i = 0; i < dx; i++) sendRGB(_background);

  DISPLAY_DISCONNECT
}

/*
void Display::symbol(byte *source, byte x, byte y, byte dx, byte dy)
{
  setAddr(x, y, x + dx - 1, y + dy - 1);
  for (char j = 0; j < dy; j++) {
    byte data = pgm_read_byte(source + j);
    for (byte i = 0; i < dx; i++) {
      if (data & 1)
        sendRGB(_color);
      else
        sendRGB(_background);
      data >>= 1;
    }
  }

  DISPLAY_DISCONNECT
}
*/

// тестирование дисплея

#define VIEWPORT_OFFSET 30
void Display::demo(byte d)
{
  setAddr(0, 0, LCD_MAX_X, LCD_MAX_Y);
  for (byte y = VIEWPORT_OFFSET; y < LCD_MAX_Y + VIEWPORT_OFFSET + 1; y++) {
    word yy = y * y;

    for (byte x = VIEWPORT_OFFSET; x < LCD_MAX_X + VIEWPORT_OFFSET + 1; x++) {
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
