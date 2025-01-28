#include "ST7789.h"

#ifdef _ST7789_

ST7789::ST7789()
{
  INIT_LCD;
  delay_ms(50); // Ждать стабилизации напряжений
  D_RST(SET);
  delay_ms(150); // Ждать стабилизации напряжений
  D_CS(CLR);
  send_command(SLPOUT);      // Проснуться
  delay_ms(10);

  send_command(COLMOD);
  send_byte(RGB_FORMAT);

  send_command(FRMCTR2);
  send_byte(0x0C);
  send_byte(0x0C);
  send_byte(0x00);
  send_byte(0x33);
  send_byte(0x33);

  send_command(MADCTL);// flip
  send_byte(0xD8);

  send_command(0XB7);
  send_byte(0x35);
  send_command(0xBB);
  send_byte(0x19);
  send_command(PWCTR1);
  send_byte(0x2C);
  send_command(PWCTR2);
  send_byte(0x01);
  send_command(PWCTR3);
  send_byte(0x12);
  send_command(PWCTR4);
  send_byte(0x20);
  send_command(0xC6);
  send_byte(0x0F);
  send_command(0xD0);
  send_byte(0xA4);
  send_byte(0xA1);

  send_command(GAMCTRP1);
  send_byte(0xD0);
  send_byte(0x04);
  send_byte(0x0D);
  send_byte(0x11);
  send_byte(0x13);
  send_byte(0x2B);
  send_byte(0x3F);
  send_byte(0x54);
  send_byte(0x4C);
  send_byte(0x18);
  send_byte(0x0D);
  send_byte(0x0B);
  send_byte(0x1F);
  send_byte(0x23);

  send_command(GAMCTRN1);
  send_byte(0xD0);
  send_byte(0x04);
  send_byte(0x0C);
  send_byte(0x11);
  send_byte(0x13);
  send_byte(0x2C);
  send_byte(0x3F);
  send_byte(0x44);
  send_byte(0x51);
  send_byte(0x2F);
  send_byte(0x1F);
  send_byte(0x1F);
  send_byte(0x20);
  send_byte(0x23);

  send_command(INVOFF);		//	Inversion OFF
  send_command(SLPOUT);	//	Out of sleep mode
  send_command(NORON);		//	Normal Display on
  send_command(DISPON);	//	Main screen turned on	
  delay_ms(50);
  D_CS(SET);
}

void ST7789::send_command(uint8_t command)
{
  D_RS(CLR);
  send_byte(command);
  D_RS(SET);
}


void ST7789::send_byte(uint8_t data)
{
  PORT(D_DATA) = data;
  D_WR(SET);
  D_WR(CLR);
}

void ST7789::send_word(uint16_t data)
{
  PORT(D_DATA) = to_byte(data, 1);
  D_WR(SET);
  D_WR(CLR);
  PORT(D_DATA) = to_byte(data, 0);
  D_WR(SET);
  D_WR(CLR);
}

void ST7789::set_addr(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
  send_command(CASET); // Column Address Set
  send_word(x0);
  send_word(x1);

  send_command(RASET); // Row Address Set
  send_word(y0);
  send_word(y1);

  send_command(RAMWR); // Memory Write
}

void ST7789::send_rgb(uint8_t r, uint8_t g, uint8_t b)
{
  PORT(D_DATA) = b;
  D_WR(SET);
  D_WR(CLR);
  PORT(D_DATA) = g;
  D_WR(SET);
  D_WR(CLR);
  PORT(D_DATA) = r;
  D_WR(SET);
  D_WR(CLR);
}

void ST7789::send_rgb(RGB color)
{
  PORT(D_DATA) = color.blue;
  D_WR(SET);
  D_WR(CLR);
  PORT(D_DATA) = color.green;
  D_WR(SET);
  D_WR(CLR);
  PORT(D_DATA) = color.red;
  D_WR(SET);
  D_WR(CLR);
}

void ST7789::rect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, RGB color)
{
  D_CS(CLR);
  set_addr(x0, y0, x1, y1);
  uint16_t x = x1 - x0;
  uint16_t y = y1 - y0;
  if (x > y) swap16(x, y);
  for (uint16_t i = 0; i <= x; i++)
    for (uint16_t j = 0; j <= y; j++) {
      PORT(D_DATA) = color.blue;
      D_WR(SET);
      D_WR(CLR);
      PORT(D_DATA) = color.green;
      D_WR(SET);
      D_WR(CLR);
      PORT(D_DATA) = color.red;
      D_WR(SET);
      D_WR(CLR);
    }

  D_CS(SET);
}

// PrintF

void ST7789::symbol(uint8_t *source, uint16_t x, uint16_t y, uint8_t dx, uint8_t dy)
{
  uint8_t sreg = SREG;
  cli();
  D_CS(CLR);

  uint16_t x1 = x + dx - 1;
  uint16_t y1 = y + dy - 1;

#ifdef FLIP_X
  uint16_t t = x;
  x = MAX_X - x1;
  x1 = MAX_X - t;
#endif

#ifdef FLIP_Y
  uint16_t u = y;
  y = MAX_Y - y1;
  y1 = MAX_Y - u;
#endif

#ifdef EX_X_Y
  set_addr(y, x, y1, x1);
#else
  set_addr(x, y, x1, y1);
#endif

#ifdef EX_X_Y

#ifdef FLIP_X
  for (char i = dx - 1; i >= 0; i--) {
  #else
  for (uint8_t i = 0; i < dx; i++) {
  #endif

    uint8_t  data;

  #ifdef FLIP_Y
    uint8_t bit = 1 << ((dy - 1) & 7);
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
    for (uint8_t j = 0; j < dy; j++) {
      if (!(j & 7)) data = pgm_read_byte((uint16_t)source + (j >> 3) * dx + i);
      if (data & 1) send_rgb(_color);
      else send_rgb(_background);
      data >>= 1;
    #endif

    }
    }

#else

#ifdef FLIP_Y
  for (char j = dy - 1; j >= 0; j--) {
  #else
  for (uint8_t j = 0; j < dy; j++) {
  #endif

    uint16_t offset = (uint16_t)source + (j >> 3) * dx;
    uint8_t bit = 1 << (j & 7);

  #ifdef FLIP_X
    for (char i = dx - 1; i >= 0; i--) {
    #else
    for (uint8_t i = 0; i < dx; i++) {
    #endif

      uint8_t data = pgm_read_byte(offset + i);
      if (data & bit) send_rgb(_color);
      else send_rgb(_background);
    }
    }

#endif

  D_CS(SET);
  SREG = sreg;
  }

void ST7789::bitmap(uint8_t * source, uint16_t x, uint16_t y, uint16_t dx, uint16_t dy)
{
  D_CS(CLR);

  uint16_t xm = x + dx;
  uint16_t ym = y + dy;

  set_addr(x, y, xm * 5 - 1, ym - 1);

  while (ym--) {
    for (uint16_t i = 0; i < xm; i++) {
      send_byte(*source);
      send_byte(*(source + 1));
      send_byte(*(source + 2));
      send_byte(*source);
      send_byte(*(source + 1));
      send_byte(*(source + 2));
      send_byte(*source);
      send_byte(*(source + 1));
      send_byte(*(source + 2));
      send_byte(*source);
      send_byte(*(source + 1));
      send_byte(*(source + 2));
      send_byte(*source);
      send_byte(*(source + 1));
      send_byte(*(source + 2));
      source++;
    }
  }

  D_CS(SET);
}


// тестирование дисплея

#define VIEWPORT_OFFSET 30


void ST7789::demo(uint8_t d)
{
  D_CS(CLR);
  set_addr(0, 0, LCD_MAX_X, LCD_MAX_Y);
  for (uint16_t y = VIEWPORT_OFFSET; y < LCD_MAX_Y + VIEWPORT_OFFSET + 1; y++) {
    uint16_t yy = y * y;

    for (uint16_t x = VIEWPORT_OFFSET; x < LCD_MAX_X + VIEWPORT_OFFSET + 1; x++) {
      uint16_t xx = x * x;

      uint8_t e = d << 2;
      uint16_t r = ((xx + yy) >> 7) + e;
      uint16_t g = ((yy - xx) >> 7) + e;
      uint16_t b = ((x * y) >> 7) - e;

      send_rgb(r, g, b);
    }
  }
  D_CS(SET);
}

#endif
