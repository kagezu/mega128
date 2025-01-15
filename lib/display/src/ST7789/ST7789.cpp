#include "ST7789.h"
#include "comands.h"

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

  send_command(MADCTL); // Memory Data Access Control
  send_byte(0xD8);



  // send_command(INVON);
  send_command(NORON);
  delay_ms(10);
  send_command(COLMOD);
  send_byte(RGB_FORMAT);
  delay_ms(10);

  send_command(DISPON); // Display On
  delay_ms(10);
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
