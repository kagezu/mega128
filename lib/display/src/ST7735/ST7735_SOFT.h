#pragma once
#include "ST7735.h"
#include "pin.h"

class ST7735_SOFT : public ST7735 {
public:
  ST7735_SOFT();

protected:
  void select() { L_CS(RES); }
  void deselect() { L_CS(SET); }

  void send_command(uint8_t data);
  void set_addr(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
  void send_zero();
  void send_byte(uint8_t data);
  void send_rgb(RGB color);
  void rect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, RGB color);
};

ST7735_SOFT::ST7735_SOFT()
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

void ST7735_SOFT::send_command(uint8_t command)
{
  L_RS(RES); // Запись команды
  send_byte(command);
  L_RS(SET); // Запись данных
};

void ST7735_SOFT::set_addr(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
  send_command(CASET); // Column Address Set
  send_zero();
  send_byte(x0);
  send_zero();
  send_byte(x1);

  send_command(RASET); // Row Address Set
  send_zero();
  send_byte(y0);
  send_zero();
  send_byte(y1);

  send_command(RAMWR); // Memory Write
};

void ST7735_SOFT::send_zero()
{
  L_SDA(RES);

  uint8_t res = L_SCK(MMO) & ~L_SCK(MASK);
  uint8_t set = L_SCK(MMO) | L_SCK(MASK);

  L_SCK(MMO) = res;
  L_SCK(MMO) = set;
  L_SCK(MMO) = res;
  L_SCK(MMO) = set;
  L_SCK(MMO) = res;
  L_SCK(MMO) = set;
  L_SCK(MMO) = res;
  L_SCK(MMO) = set;
  L_SCK(MMO) = res;
  L_SCK(MMO) = set;
  L_SCK(MMO) = res;
  L_SCK(MMO) = set;
  L_SCK(MMO) = res;
  L_SCK(MMO) = set;
  L_SCK(MMO) = res;
  L_SCK(MMO) = set;
};

void ST7735_SOFT::send_byte(uint8_t data)
{
  uint8_t b0 = L_SCK(MMO) & ~(L_SDA(MASK) | L_SCK(MASK));
  uint8_t b1 = (L_SCK(MMO) | L_SDA(MASK)) & ~L_SCK(MASK);
  uint8_t set = L_SCK(MMO) | L_SCK(MASK);

  L_SCK(MMO) = data & 0x80 ? b1 : b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = data & 0x40 ? b1 : b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = data & 0x20 ? b1 : b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = data & 0x10 ? b1 : b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = data & 0x8 ? b1 : b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = data & 0x4 ? b1 : b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = data & 0x2 ? b1 : b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = data & 0x1 ? b1 : b0;
  L_SCK(MMO) = set;
};


void ST7735_SOFT::send_rgb(RGB color)
{
  uint8_t r = color.red;
  uint8_t g = color.green;
  uint8_t b = color.blue;

  uint8_t b0 = L_SCK(MMO) & ~(L_SDA(MASK) | L_SCK(MASK));
  uint8_t b1 = (L_SCK(MMO) | L_SDA(MASK)) & ~L_SCK(MASK);
  uint8_t set = L_SCK(MMO) | L_SCK(MASK);

  L_SCK(MMO) = r & 0x80 ? b1 : b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = r & 0x40 ? b1 : b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = r & 0x20 ? b1 : b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = r & 0x10 ? b1 : b0;
  L_SCK(MMO) = set;
#if RGB_FORMAT == RGB_18 || RGB_FORMAT == RGB_16
  L_SCK(MMO) = r & 0x8 ? b1 : b0;
  L_SCK(MMO) = set;
#endif
#if RGB_FORMAT == RGB_18
  L_SCK(MMO) = r & 0x4 ? b1 : b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = b0;
  L_SCK(MMO) = set;
#endif

  L_SCK(MMO) = g & 0x80 ? b1 : b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = g & 0x40 ? b1 : b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = g & 0x20 ? b1 : b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = g & 0x10 ? b1 : b0;
  L_SCK(MMO) = set;
#if RGB_FORMAT == RGB_18 || RGB_FORMAT == RGB_16
  L_SCK(MMO) = g & 0x8 ? b1 : b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = g & 0x4 ? b1 : b0;
  L_SCK(MMO) = set;
#endif
#if RGB_FORMAT == RGB_18
  L_SCK(MMO) = b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = b0;
  L_SCK(MMO) = set;
#endif

  L_SCK(MMO) = b & 0x80 ? b1 : b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = b & 0x40 ? b1 : b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = b & 0x20 ? b1 : b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = b & 0x10 ? b1 : b0;
  L_SCK(MMO) = set;
#if RGB_FORMAT == RGB_18 || RGB_FORMAT == RGB_16
  L_SCK(MMO) = b & 0x8 ? b1 : b0;
  L_SCK(MMO) = set;
#endif
#if RGB_FORMAT == RGB_18
  L_SCK(MMO) = b & 0x4 ? b1 : b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = b0;
  L_SCK(MMO) = set;
#endif
};

void ST7735_SOFT::rect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, RGB color)
{
  uint8_t r = color.red;
  uint8_t g = color.green;
  uint8_t b = color.blue;

  L_CS(RES);
  set_addr(x0, y0, x1, y1);
  uint16_t len = (x1 - x0 + 1) * (y1 - y0 + 1);

  uint8_t b0 = L_SCK(MMO) & ~(L_SDA(MASK) | L_SCK(MASK));
  uint8_t b1 = (L_SCK(MMO) | L_SDA(MASK)) & ~L_SCK(MASK);
  uint8_t set = L_SCK(MMO) | L_SCK(MASK);

  // Дублирование кода намеренно, так как оптимизатор ускоряет тут выполнение в 2 раза
  while (len--) {
    L_SCK(MMO) = r & 0x80 ? b1 : b0;
    L_SCK(MMO) = set;
    L_SCK(MMO) = r & 0x40 ? b1 : b0;
    L_SCK(MMO) = set;
    L_SCK(MMO) = r & 0x20 ? b1 : b0;
    L_SCK(MMO) = set;
    L_SCK(MMO) = r & 0x10 ? b1 : b0;
    L_SCK(MMO) = set;
  #if RGB_FORMAT == RGB_18 || RGB_FORMAT == RGB_16
    L_SCK(MMO) = r & 0x8 ? b1 : b0;
    L_SCK(MMO) = set;
  #endif
  #if RGB_FORMAT == RGB_18
    L_SCK(MMO) = r & 0x4 ? b1 : b0;
    L_SCK(MMO) = set;
    L_SCK(MMO) = b0;
    L_SCK(MMO) = set;
    L_SCK(MMO) = b0;
    L_SCK(MMO) = set;
  #endif

    L_SCK(MMO) = g & 0x80 ? b1 : b0;
    L_SCK(MMO) = set;
    L_SCK(MMO) = g & 0x40 ? b1 : b0;
    L_SCK(MMO) = set;
    L_SCK(MMO) = g & 0x20 ? b1 : b0;
    L_SCK(MMO) = set;
    L_SCK(MMO) = g & 0x10 ? b1 : b0;
    L_SCK(MMO) = set;
  #if RGB_FORMAT == RGB_18 || RGB_FORMAT == RGB_16
    L_SCK(MMO) = g & 0x8 ? b1 : b0;
    L_SCK(MMO) = set;
    L_SCK(MMO) = g & 0x4 ? b1 : b0;
    L_SCK(MMO) = set;
  #endif
  #if RGB_FORMAT == RGB_18
    L_SCK(MMO) = b0;
    L_SCK(MMO) = set;
    L_SCK(MMO) = b0;
    L_SCK(MMO) = set;
  #endif

    L_SCK(MMO) = b & 0x80 ? b1 : b0;
    L_SCK(MMO) = set;
    L_SCK(MMO) = b & 0x40 ? b1 : b0;
    L_SCK(MMO) = set;
    L_SCK(MMO) = b & 0x20 ? b1 : b0;
    L_SCK(MMO) = set;
    L_SCK(MMO) = b & 0x10 ? b1 : b0;
    L_SCK(MMO) = set;
  #if RGB_FORMAT == RGB_18 || RGB_FORMAT == RGB_16
    L_SCK(MMO) = b & 0x8 ? b1 : b0;
    L_SCK(MMO) = set;
  #endif
  #if RGB_FORMAT == RGB_18
    L_SCK(MMO) = b & 0x4 ? b1 : b0;
    L_SCK(MMO) = set;
    L_SCK(MMO) = b0;
    L_SCK(MMO) = set;
    L_SCK(MMO) = b0;
    L_SCK(MMO) = set;
  #endif
  }
  L_CS(SET);
};

/*
void ST7735_SOFT::send_rgb(uint16_t data)
{
  uint8_t b0 = L_SCK(MMO) & ~(L_SDA(MASK) | L_SCK(MASK));
  uint8_t b1 = (L_SCK(MMO) | L_SDA(MASK)) & ~L_SCK(MASK);
  uint8_t set = L_SCK(MMO) | L_SCK(MASK);

#if RGB_FORMAT == RGB_18 || RGB_FORMAT == RGB_16
  L_SCK(MMO) = data & 0x8000 ? b1 : b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = data & 0x4000 ? b1 : b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = data & 0x2000 ? b1 : b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = data & 0x1000 ? b1 : b0;
  L_SCK(MMO) = set;
#endif

  L_SCK(MMO) = data & 0x800 ? b1 : b0;
  L_SCK(MMO) = set;

#if RGB_FORMAT == RGB_18
  L_SCK(MMO) = data & 0x8000 ? b1 : b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = data & 0x8000 ? b1 : b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = data & 0x8000 ? b1 : b0;
  L_SCK(MMO) = set;
#endif

  L_SCK(MMO) = data & 0x400 ? b1 : b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = data & 0x200 ? b1 : b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = data & 0x100 ? b1 : b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = data & 0x80 ? b1 : b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = data & 0x40 ? b1 : b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = data & 0x20 ? b1 : b0;
  L_SCK(MMO) = set;

#if RGB_FORMAT == RGB_18
  L_SCK(MMO) = data & 0x400 ? b1 : b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = data & 0x400 ? b1 : b0;
  L_SCK(MMO) = set;
#endif

  L_SCK(MMO) = data & 0x10 ? b1 : b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = data & 0x8 ? b1 : b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = data & 0x4 ? b1 : b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = data & 0x2 ? b1 : b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = data & 0x1 ? b1 : b0;
  L_SCK(MMO) = set;

#if RGB_FORMAT == RGB_18
  L_SCK(MMO) = data & 0x10 ? b1 : b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = data & 0x10 ? b1 : b0;
  L_SCK(MMO) = set;
  L_SCK(MMO) = data & 0x10 ? b1 : b0;
  L_SCK(MMO) = set;
#endif
};
*/
