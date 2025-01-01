#include <avr/pgmspace.h>
#include "ST7735S_SPI.h"

ST7735S_SPI::ST7735S_SPI()
{
  INIT_LCD;
  INIT_SPI;
  delayMicroseconds(15000); // Ждать стабилизации напряжений
  DISPLAY_CONNECT;          // CS Выбор дисплея

  send_command(SLPOUT);      // Проснуться
  delayMicroseconds(15000); // Ждать стабилизации напряжений

  send_command(FRMCTR1); // In normal mode (Full colors)
  send_byte(0x05);
  send_byte(0x3C);
  send_byte(0x3C);

  send_command(FRMCTR2); // In Idle mode (8-colors)
  send_byte(0x05);
  send_byte(0X3C);
  send_byte(0X3C);

  send_command(FRMCTR3); // In partial mode + Full colors
  send_byte(0x05);
  send_byte(0x3C);
  send_byte(0x3C);
  send_byte(0x05);
  send_byte(0x3C);
  send_byte(0x3C);

  send_command(INVCTR); // Display inversion control
  send_byte(0x03);

  send_command(PWCTR1); // Power control setting
  send_byte(0x28);
  send_byte(0x08);
  send_byte(0x04);

  send_command(PWCTR2); // Power control setting
  send_byte(0xC0);

  send_command(PWCTR3); // In normal mode (Full colors)
  send_byte(0x0D);
  send_byte(0X00);

  send_command(PWCTR4); // In Idle mode (8-colors)
  send_byte(0x8D);
  send_byte(0x2A);

  send_command(PWCTR5); // In partial mode + Full colors
  send_byte(0x8D);
  send_byte(0xEE);

  send_command(VMCTR1); // VCOM control 1
  send_byte(0x1A);

  send_command(0x17); // ?
  send_byte(0x05);

  send_command(MADCTL); // Memory Data Access Control
  send_byte(0xD8);

  send_command(GAMCTRP1); // Set Gamma adjustment (+ polarity)
  send_byte(0x03);
  send_byte(0x22);
  send_byte(0x07);
  send_byte(0x0A);
  send_byte(0x2E);
  send_byte(0x30);
  send_byte(0x25);
  send_byte(0x2A);
  send_byte(0x28);
  send_byte(0x26);
  send_byte(0x2E);
  send_byte(0x3A);
  send_byte(0x00);
  send_byte(0x01);
  send_byte(0x03);
  send_byte(0x13);

  send_command(GAMCTRN1); // Set Gamma adjustment (- polarity)
  send_byte(0x04);
  send_byte(0x16);
  send_byte(0x06);
  send_byte(0x0D);
  send_byte(0x2D);
  send_byte(0x26);
  send_byte(0x23);
  send_byte(0x27);
  send_byte(0x27);
  send_byte(0x25);
  send_byte(0x2D);
  send_byte(0x3B);
  send_byte(0x00);
  send_byte(0x01);
  send_byte(0x04);
  send_byte(0x13);

  send_command(COLMOD);
  send_byte(RGB_FORMAT);

  send_command(DISPON); // Display On
  DISPLAY_DISCONNECT
}

void ST7735S_SPI::send_command(byte command)
{
  SPI_WAIT;
  COMMAND_MODE; // Запись команды
  SPDR = command;
  asm volatile("nop");
  SPI_WAIT;
  DATA_MODE // Запись данных
};

void ST7735S_SPI::set_addr(byte x0, byte y0, byte x1, byte y1)
{
  send_command(CASET); // Column Address Set
  SPDR = 0;
  send_byte(x0);
  SPI_WAIT;
  SPDR = 0;
  send_byte(x1);

  send_command(RASET); // Row Address Set
  SPDR = 0;
  send_byte(y0);
  SPI_WAIT;
  SPDR = 0;
  send_byte(y1);

  send_command(RAMWR); // Memory Write
};

void ST7735S_SPI::send_byte(byte data)
{
  SPI_WAIT;
  SPDR = data;
};

#if RGB_FORMAT == RGB_12
void ST7735S_SPI::send_rgb(uint16_t color)
{
  static byte half, flag = 0;
  byte data;

  if (flag) {
    data = half | (color >> 8);
    SPI_WAIT;
    SPDR = data;
    flag = 0;
    SPI_WAIT;
    SPDR = color;
  }
  else {
    data = color >> 4;
    SPI_WAIT;
    SPDR = data;
    half = color << 4;
    flag = 1;
  }
};

#elif RGB_FORMAT == RGB_16
void ST7735S_SPI::send_rgb(uint16_t data)
{
  SPI_WAIT;
  SPDR = data >> 8;
  asm volatile("nop");
  SPI_WAIT;
  SPDR = data;
};

#elif RGB_FORMAT == RGB_18
void ST7735S_SPI::send_rgb(uint16_t data) // формат 0x0rgb
{
  send_rgb((data >> 4) & 0xf0, data & 0xf0, data << 4);
}
#endif

void ST7735S_SPI::send_rgb(uint32_t color)
{
  send_rgb(color >> 16, color >> 8, color);
}

void ST7735S_SPI::send_rgb(RGB color)
{
#if RGB_FORMAT == RGB_12 || RGB_FORMAT == RGB_16
  send_rgb((uint16_t)color);
#elif RGB_FORMAT == RGB_18
  send_rgb(color.red, color.green, color.blue);
#endif
}

void ST7735S_SPI::send_rgb(byte r, byte g, byte b)
{

#if RGB_FORMAT == RGB_12

  static byte half, flag = 0;
  byte data;
  if (flag) {
    data = half | (b >> 4);
    SPI_WAIT;
    SPDR = data;
    flag = 0;
    data = (g & 0xf0) | (r >> 4);
    SPI_WAIT;
    SPDR = data;
  }
  else {
    data = (b & 0xf0) | (g >> 4);
    SPI_WAIT;
    SPDR = data;
    half = r & 0xf0;
    flag = 1;
  }

#elif RGB_FORMAT == RGB_16

  byte data = (g >> 5) | (b & 0xf8);
  SPI_WAIT;
  SPDR = data;

  data = ((g & 0x1c) << 3) | (r >> 3);
  SPI_WAIT;
  SPDR = data;

#elif RGB_FORMAT == RGB_18

  SPI_WAIT;
  SPDR = b;
  asm volatile("nop");
  SPI_WAIT;
  SPDR = g;
  asm volatile("nop");
  SPI_WAIT;
  SPDR = r;

#endif


};

void ST7735S_SPI::rect(byte x0, byte y0, byte x1, byte y1, RGB color)
{
  DISPLAY_CONNECT;

  word len = (x1 - x0 + 1) * (y1 - y0 + 1);
  set_addr(x0, y0, x1, y1);

#if RGB_FORMAT == RGB_12

  byte hByte = color >> 4;
  byte mByte = (color << 4) | ((color & 0xf00) >> 8);
  byte lByte = color;
  len >>= 1;

#elif RGB_FORMAT == RGB_16

  byte hByte = (word)color >> 8;
  byte lByte = (word)color;

#endif

  while (len--) {

  #if RGB_FORMAT == RGB_12

    SPI_WAIT;
    SPDR = hByte;
    asm volatile("nop");
    SPI_WAIT;
    SPDR = mByte;
    asm volatile("nop");
    SPI_WAIT;
    SPDR = lByte;

  #elif RGB_FORMAT == RGB_16

    SPI_WAIT;
    SPDR = hByte;
    asm volatile("nop");
    SPI_WAIT;
    SPDR = lByte;

  #elif RGB_FORMAT == RGB_18

    SPI_WAIT;
    SPDR = color.blue;
    asm volatile("nop");
    SPI_WAIT;
    SPDR = color.green;
    asm volatile("nop");
    SPI_WAIT;
    SPDR = color.red;

  #endif

  }

  DISPLAY_DISCONNECT
};
