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
  sendByte(0);
  sendByte(0);
  sendRGB(color);
#elif RGB_FORMAT == RGB_18
  sendByte(0);
  sendByte(0);
  sendByte(0);
  sendRGB(color);
#endif
  DISPLAY_DISCONNECT;
}

void Display::rectFill(uint8_t x, uint8_t y, uint8_t x1, uint8_t y1)
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

void Display::clear(RGB color)
{
  rect(0, 0, LCD_MAX_X, LCD_MAX_Y, color);
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
  setAddr(0, 0, LCD_MAX_X, LCD_MAX_Y);

  // 0°
#if !FLIP_X && !FLIP_Y && !EX_X_Y
  uint16_t length = (LCD_MAX_X + 1) * (LCD_MAX_Y + 1);
  while (length--) sendRGB(*source++);
#endif

  // 180°
#if FLIP_X && FLIP_Y && !EX_X_Y
  uint16_t length = (LCD_MAX_X + 1) * (LCD_MAX_Y + 1);
  source += length - 1;
  while (length--) sendRGB(*source--);
#endif

  // 90°
#if !FLIP_X && FLIP_Y && EX_X_Y
  RGB *ptr;
  source += LCD_MAX_X * (LCD_MAX_Y + 1);
  for (byte j = 0; j < LCD_MAX_Y + 1; j++) {
    ptr = source + j;
    for (byte i = 0; i < LCD_MAX_X + 1; i++) {
      sendRGB(*source);
      ptr -= LCD_MAX_Y + 1;
    }
  }
#endif

  // 270°
#if FLIP_X && !FLIP_Y && EX_X_Y
  RGB *ptr;
  source += LCD_MAX_Y;
  for (byte j = 0; j < LCD_MAX_Y + 1; j++) {
    ptr = source - j;
    for (byte i = 0; i < LCD_MAX_X + 1; i++) {
      sendRGB(*source);
      ptr += LCD_MAX_Y + 1;
    }
  }
#endif

  DISPLAY_DISCONNECT;
}

void Display::symbol(byte *source, byte x, byte y, byte dx, byte dy)
{
  byte x1 = x + dx - 1;
  byte y1 = y + dy - 1;

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
  setAddr(y, x, y1, x1);
#else
  setAddr(x, y, x1, y1);
#endif

#if EX_X_Y

#if FLIP_X
  for (char i = dx - 1; i >= 0; i--) {
  #else
  for (byte i = 0; i < dx; i++) {
  #endif

    byte  data;

  #if FLIP_Y
    byte bit = 1 << ((dy - 1) & 7);
    data = pgm_read_byte((word)source + ((dy - 1) >> 3) * dx + i);
    for (char j = dy - 1; j >= 0; j--) {
      if ((j & 7) == 7) {
        data = pgm_read_byte((word)source + (j >> 3) * dx + i);
        bit = 128;
      }
      if (data & bit) sendRGB(_color);
      else sendRGB(_background);
      bit >>= 1;
    #else
    for (byte j = 0; j < dy; j++) {
      if (!(j & 7)) data = pgm_read_byte((word)source + (j >> 3) * dx + i);
      if (data & 1) sendRGB(_color);
      else sendRGB(_background);
      data >>= 1;
    #endif

    }
    }

#else

#if FLIP_Y
  for (char j = dy - 1; j >= 0; j--) {
  #else
  for (byte j = 0; j < dy; j++) {
  #endif

    word offset = (word)source + (j >> 3) * dx;
    byte bit = 1 << (j & 7);

  #if FLIP_X
    for (char i = dx - 1; i >= 0; i--) {
    #else
    for (byte i = 0; i < dx; i++) {
    #endif

      byte data = pgm_read_byte(offset + i);
      if (data & bit) sendRGB(_color);
      else sendRGB(_background);
    }
  }

#endif

  DISPLAY_DISCONNECT
}

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

void Display::test(byte k)
{
  setAddr(0, 0, LCD_MAX_X, LCD_MAX_X);
  for (byte y = 0; y < LCD_MAX_Y + 1; y++)
    for (byte x = 0; x < LCD_MAX_X + 1; x++) {

      word r = x << 1;
      word g = y << 1;
      word b = k;

      sendRGB(RGB(r, g, b));
      // sendRGB(((uint32_t)r << 16) | (g << 8) | b);
    }
  DISPLAY_DISCONNECT
}
