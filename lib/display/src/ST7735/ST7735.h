#pragma once
#include "display/default.h"
#include "ST7735/ST7735_SPI.h"
#include "ST7735_SOFT.h"
#include "print/print-format.h"
#include "draw/draw.h"

class ST7735
#ifdef LCD_SPI
  : public ST7735_SPI
#elif defined ( LCD_SOFT)
  : public ST7735_SOFT
#endif
  , public PrintF, public Draw {
  public:
    ST7735();

  public:
    void color(RGB c) { _color = c; }
    void background(RGB b) { _background = b; }
    void clear() { rect(0, 0, MAX_X, MAX_Y, _background); }
    void clear(RGB color) { rect(0, 0, MAX_X, MAX_Y, color); }
    void rect_fill(byte x, byte y, byte x1, byte y1) { rect(x, y, x1, y1, _color); }

    void symbol(byte *, uint16_t, uint16_t, byte, byte);
    void pixel(byte, byte);

    // Скринсейвер
    void demo(byte);
    void test(byte);

  private:
    RGB _color = 0x00ffffff;
    RGB _background = 0;

    void send_config(const byte *, byte);
};

void ST7735::send_config(const byte *config, byte size)
{
  while (size) {
    byte data, comand = pgm_read_byte(config++);
    size -= 2;
    send_command(comand);
    while ((data = pgm_read_byte(config++)) != 0xFF) {
      send_byte(data);
      size--;
    }
  }
}

ST7735::ST7735()
{
  L_SCK(OUT);
  L_SDA(OUT);
  L_RST(OUT);
  L_CS(OUT);
  L_RS(OUT);

  L_RST(SET);
  delay_ms(20); // Ждать стабилизации напряжений
  L_CS(RES);          // CS Выбор дисплея
  send_command(SLPOUT);      // Проснуться
  delay_ms(20); // Ждать стабилизации напряжений

  send_config(ST7735_CONFIG, sizeof(ST7735_CONFIG));

  send_command(MADCTL);
  send_byte(
  #ifdef EX_X_Y
    0x20 |
  #endif
  #ifdef FLIP_X
    0x40 |
  #endif
  #ifdef FLIP_Y
    0x80 |
  #endif
    0x00);

  send_command(COLMOD);
  send_byte(RGB_FORMAT);
  send_command(DISPON); // Display On

  L_CS(SET);
}

// Реализация виртуальных методов класса GFX

void ST7735::pixel(byte x, byte y)
{
  if (x > MAX_X || y > MAX_Y) return;
  L_CS(RES);

  set_addr(x, y, x, y);
  send_rgb(_color);
  send_rgb(_color);

  L_CS(SET);
}


// Реализация интерфейса PrintF

void ST7735::symbol(byte *source, uint16_t x, uint16_t y, byte dx, byte dy)
{
  byte sreg = SREG;
  cli();
  L_CS(RES);

  uint16_t x1 = x + dx - 1;
  uint16_t y1 = y + dy - 1;
  set_addr(x, y, x1, y1);

  for (byte j = 0; j < dy; j++) {
    uint16_t offset = (uint16_t)source + (j >> 3) * dx;
    byte bit = 1 << (j & 7);
    for (byte i = 0; i < dx; i++) {
      byte data = pgm_read_byte(offset + i);
      if (data & bit) send_rgb(_color);
      else send_rgb(_background);
    }
  }

  L_CS(SET);
  SREG = sreg;
}

// тестирование дисплея

#define VIEWPORT_OFFSET 30

void ST7735::demo(byte d)
{
  L_CS(RES);
  set_addr(0, 0, MAX_X, MAX_Y);
  for (byte y = VIEWPORT_OFFSET; y < MAX_Y + VIEWPORT_OFFSET + 1; y++) {
    uint16_t yy = y * y;

    for (byte x = VIEWPORT_OFFSET; x < MAX_X + VIEWPORT_OFFSET + 1; x++) {
      uint16_t xx = x * x;

      byte e = d << 2;
      uint16_t r = ((xx + yy) >> 6) + e;
      uint16_t g = ((yy - xx) >> 6) + e;
      uint16_t b = ((x * y) >> 6) - e;

      send_rgb(RGB(r, g, b));
    }
  }
  L_CS(SET);
}

void ST7735::test(byte k)
{
  L_CS(RES);
  set_addr(0, 0, MAX_X, MAX_Y);
  for (byte y = 0; y < MAX_Y + 1; y++)
    for (byte x = 0; x < MAX_X + 1; x++) {
      send_rgb(RGB(x << 1, y << 1, k));
    }
  L_CS(SET);
}
