#include "ST7735.h"

void ST7735::send_config(const uint8_t *config, uint8_t size)
{
  while (size) {
    uint8_t data, comand = pgm_read_byte(config++);
    size -= 2;
    send_command(comand);
    while ((data = pgm_read_byte(config++)) != 0xFF) {
      send_byte(data);
      size--;
    }
  }
}

// Реализация виртуальных методов класса GFX

void ST7735::pixel(uint8_t x, uint8_t y)
{
  if (x > MAX_X || y > MAX_Y) return;
  select();

  set_addr(x, y, x, y);
  send_rgb(_color);
  send_rgb(_color);

  deselect();
}


// Реализация интерфейса PrintF

void ST7735::symbol(uint8_t *source, uint16_t x, uint16_t y, uint8_t dx, uint8_t dy)
{
  uint8_t sreg = SREG;
  cli();
  select();

  uint16_t x1 = x + dx - 1;
  uint16_t y1 = y + dy - 1;
  set_addr(x, y, x1, y1);

  for (uint8_t j = 0; j < dy; j++) {
    uint16_t offset = (uint16_t)source + (j >> 3) * dx;
    uint8_t bit = 1 << (j & 7);
    for (uint8_t i = 0; i < dx; i++) {
      uint8_t data = pgm_read_byte(offset + i);
      if (data & bit) send_rgb(_color);
      else send_rgb(_background);
    }
  }

  deselect();
  SREG = sreg;
}

// тестирование дисплея

#define VIEWPORT_OFFSET 30

void ST7735::demo(uint8_t d)
{
  select();
  set_addr(0, 0, MAX_X, MAX_Y);
  for (uint8_t y = VIEWPORT_OFFSET; y < MAX_Y + VIEWPORT_OFFSET + 1; y++) {
    uint16_t yy = y * y;

    for (uint8_t x = VIEWPORT_OFFSET; x < MAX_X + VIEWPORT_OFFSET + 1; x++) {
      uint16_t xx = x * x;

      uint8_t e = d << 2;
      uint16_t r = ((xx + yy) >> 6) + e;
      uint16_t g = ((yy - xx) >> 6) + e;
      uint16_t b = ((x * y) >> 6) - e;

      send_rgb(RGB(r, g, b));
    }
  }
  deselect();
}

void ST7735::test(uint8_t k)
{
  select();
  set_addr(0, 0, MAX_X, MAX_Y);
  for (uint8_t y = 0; y < MAX_Y + 1; y++)
    for (uint8_t x = 0; x < MAX_X + 1; x++) {
      send_rgb(RGB(x << 1, y << 1, k));
    }
  deselect();
}
