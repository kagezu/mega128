#include "display.h"

// Реализация виртуальных методов класса Draw

void Display::pixel(byte x, byte y)
{
  if (x > MAX_X || y > MAX_Y) return;
  DISPLAY_CONNECT;

#if FLIP_X
  x = MAX_X - x;
#endif

#if FLIP_Y
  y = MAX_Y - y;
#endif

#if EX_X_Y
  set_addr(y, x, y, x);
#else
  set_addr(x, y, x, y);
#endif

#if RGB_FORMAT == RGB_12
  send_rgb((uint16_t)0);
  send_rgb(_color);
#elif RGB_FORMAT == RGB_16
  send_byte(0);
  send_byte(0);
  send_rgb(_color);
#elif RGB_FORMAT == RGB_18
  send_byte(0);
  send_byte(0);
  send_byte(0);
  send_rgb(_color);
#endif

  DISPLAY_DISCONNECT;
}

void Display::rect_fill(uint8_t x, uint8_t y, uint8_t x1, uint8_t y1)
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

/*
void Display::scan_bitmap(uint8_t x, uint8_t y, uint8_t width, uint8_t height, RGB *source)
{
  set_addr(x, y, x + width - 1, y + height - 1);
  uint16_t length = width * height;
  while (length--)
    send_rgb(*source++);

  DISPLAY_DISCONNECT;
}

void Display::scan_bitmap(RGB *source)
{
  set_addr(0, 0, LCD_MAX_X, LCD_MAX_Y);

  // 0°
#if !FLIP_X && !FLIP_Y && !EX_X_Y
  uint16_t length = (LCD_MAX_X + 1) * (LCD_MAX_Y + 1);
  while (length--) send_rgb(*source++);
#endif

  // 180°
#if FLIP_X && FLIP_Y && !EX_X_Y
  uint16_t length = (LCD_MAX_X + 1) * (LCD_MAX_Y + 1);
  source += length - 1;
  while (length--) send_rgb(*source--);
#endif

  // 90°
#if !FLIP_X && FLIP_Y && EX_X_Y
  RGB *ptr;
  source += LCD_MAX_X * (LCD_MAX_Y + 1);
  for (byte j = 0; j < LCD_MAX_Y + 1; j++) {
    ptr = source + j;
    for (byte i = 0; i < LCD_MAX_X + 1; i++) {
      send_rgb(*source);
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
      send_rgb(*source);
      ptr += LCD_MAX_Y + 1;
    }
  }
#endif

  DISPLAY_DISCONNECT;
}
*/

void Display::symbol(byte *source, byte x, byte y, byte dx, byte dy)
{
  byte sreg = SREG;
  cli();
  DISPLAY_CONNECT;

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
  set_addr(y, x, y1, x1);
#else
  set_addr(x, y, x1, y1);
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
    data = pgm_read_byte((uint16_t)source + ((dy - 1) >> 3) * dx + i);
    for (char j = dy - 1; j >= 0; j--) {
      if ((j & 7) == 7) {
        data = pgm_read_byte((uint16_t)source + (j >> 3) * dx + i);
        bit = 128;
      }
      if (data & bit) send_rgb(_color);
      else send_rgb(_background);
      bit >>= 1;
    #else
    for (byte j = 0; j < dy; j++) {
      if (!(j & 7)) data = pgm_read_byte((uint16_t)source + (j >> 3) * dx + i);
      if (data & 1) send_rgb(_color);
      else send_rgb(_background);
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

    uint16_t offset = (uint16_t)source + (j >> 3) * dx;
    byte bit = 1 << (j & 7);

  #if FLIP_X
    for (char i = dx - 1; i >= 0; i--) {
    #else
    for (byte i = 0; i < dx; i++) {
    #endif

      byte data = pgm_read_byte(offset + i);
      if (data & bit) send_rgb(_color);
      else send_rgb(_background);
    }
    }

#endif

  DISPLAY_DISCONNECT;
  SREG = sreg;
  }

// тестирование дисплея

#define VIEWPORT_OFFSET 30

void Display::demo(byte d)
{
  DISPLAY_CONNECT;
  set_addr(0, 0, LCD_MAX_X, LCD_MAX_Y);
  for (byte y = VIEWPORT_OFFSET; y < LCD_MAX_Y + VIEWPORT_OFFSET + 1; y++) {
    uint16_t yy = y * y;

    for (byte x = VIEWPORT_OFFSET; x < LCD_MAX_X + VIEWPORT_OFFSET + 1; x++) {
      uint16_t xx = x * x;

      byte e = d << 2;
      uint16_t r = ((xx + yy) >> 6) + e;
      uint16_t g = ((yy - xx) >> 6) + e;
      uint16_t b = ((x * y) >> 6) - e;

      send_rgb(r, g, b);
    }
  }
  DISPLAY_DISCONNECT
}

void Display::test(byte k)
{
  DISPLAY_CONNECT;
  set_addr(0, 0, LCD_MAX_X, LCD_MAX_Y);
  for (byte y = 0; y < LCD_MAX_Y + 1; y++)
    for (byte x = 0; x < LCD_MAX_X + 1; x++) {

      // uint16_t r = x << 1;
      // uint16_t g = y << 1;
      // uint16_t b = k;

      // send_rgb(RGB(r, g, b));
      // send_rgb(r, g, b);
      send_rgb(x << 1, y << 1, k);
    }
  DISPLAY_DISCONNECT
}
