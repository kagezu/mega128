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

  send_command(INVON);		//	Inversion ON
  send_command(SLPOUT);	//	Out of sleep mode
  send_command(NORON);		//	Normal Display on
  send_command(DISPON);	//	Main screen turned on	
  delay_ms(50);
  D_CS(SET);
}

#endif

void ST7789::send_command(byte command)
{
  D_RS(CLR);
  send_byte(command);
  D_RS(SET);
}


void ST7789::send_byte(byte data)
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

void ST7789::send_rgb(byte r, byte g, byte b)
{
  PORT(D_DATA) = r;
  D_WR(SET);
  D_WR(CLR);
  PORT(D_DATA) = g;
  D_WR(SET);
  D_WR(CLR);
  PORT(D_DATA) = b;
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
    for (uint16_t j = 0; i <= y; j++) {
      PORT(D_DATA) = color.red;
      D_WR(SET);
      D_WR(CLR);
      PORT(D_DATA) = color.green;
      D_WR(SET);
      D_WR(CLR);
      PORT(D_DATA) = color.blue;
      D_WR(SET);
      D_WR(CLR);
    }

  D_CS(SET);
}

// тестирование дисплея

#define VIEWPORT_OFFSET 30

void ST7789::demo(byte d)
{
  D_CS(CLR);
  set_addr(0, 0, LCD_MAX_X, LCD_MAX_Y);
  for (uint16_t y = VIEWPORT_OFFSET; y < LCD_MAX_Y + VIEWPORT_OFFSET + 1; y++) {
    uint16_t yy = y * y;

    for (uint16_t x = VIEWPORT_OFFSET; x < LCD_MAX_X + VIEWPORT_OFFSET + 1; x++) {
      uint16_t xx = x * x;

      byte e = d << 2;
      uint16_t r = ((xx + yy) >> 7) + e;
      uint16_t g = ((yy - xx) >> 7) + e;
      uint16_t b = ((x * y) >> 7) - e;

      send_rgb(r, g, b);
    }
  }
  D_CS(SET);
}
