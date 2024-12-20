#include <avr/pgmspace.h>
#include "ST7735S_SPI.h"

ST7735S_SPI::ST7735S_SPI()
{
  INIT_LCD
    delayMicroseconds(15000); // Ждать стабилизации напряжений
  DISPLAY_CONNECT;          // CS Выбор дисплея

  sendCommand(SLPOUT);      // Проснуться
  delayMicroseconds(15000); // Ждать стабилизации напряжений

  sendCommand(FRMCTR1); // In normal mode (Full colors)
  sendByte(0x05);
  sendByte(0x3C);
  sendByte(0x3C);

  sendCommand(FRMCTR2); // In Idle mode (8-colors)
  sendByte(0x05);
  sendByte(0X3C);
  sendByte(0X3C);

  sendCommand(FRMCTR3); // In partial mode + Full colors
  sendByte(0x05);
  sendByte(0x3C);
  sendByte(0x3C);
  sendByte(0x05);
  sendByte(0x3C);
  sendByte(0x3C);

  sendCommand(INVCTR); // Display inversion control
  sendByte(0x03);

  sendCommand(PWCTR1); // Power control setting
  sendByte(0x28);
  sendByte(0x08);
  sendByte(0x04);

  sendCommand(PWCTR2); // Power control setting
  sendByte(0xC0);

  sendCommand(PWCTR3); // In normal mode (Full colors)
  sendByte(0x0D);
  sendByte(0X00);

  sendCommand(PWCTR4); // In Idle mode (8-colors)
  sendByte(0x8D);
  sendByte(0x2A);

  sendCommand(PWCTR5); // In partial mode + Full colors
  sendByte(0x8D);
  sendByte(0xEE);

  sendCommand(VMCTR1); // VCOM control 1
  sendByte(0x1A);

  sendCommand(0x17); // ?
  sendByte(0x05);

  sendCommand(MADCTL); // Memory Data Access Control
  sendByte(0xD8);

  sendCommand(GAMCTRP1); // Set Gamma adjustment (+ polarity)
  sendByte(0x03);
  sendByte(0x22);
  sendByte(0x07);
  sendByte(0x0A);
  sendByte(0x2E);
  sendByte(0x30);
  sendByte(0x25);
  sendByte(0x2A);
  sendByte(0x28);
  sendByte(0x26);
  sendByte(0x2E);
  sendByte(0x3A);
  sendByte(0x00);
  sendByte(0x01);
  sendByte(0x03);
  sendByte(0x13);

  sendCommand(GAMCTRN1); // Set Gamma adjustment (- polarity)
  sendByte(0x04);
  sendByte(0x16);
  sendByte(0x06);
  sendByte(0x0D);
  sendByte(0x2D);
  sendByte(0x26);
  sendByte(0x23);
  sendByte(0x27);
  sendByte(0x27);
  sendByte(0x25);
  sendByte(0x2D);
  sendByte(0x3B);
  sendByte(0x00);
  sendByte(0x01);
  sendByte(0x04);
  sendByte(0x13);

  sendCommand(COLMOD);
  sendByte(RGB_FORMAT);

  sendCommand(DISPON); // Display On
  DISPLAY_DISCONNECT

}

void ST7735S_SPI::sendCommand(byte command)
{
  SPI_WAIT;
  COMMAND_MODE; // Запись команды
  SPDR = command;
  SPI_WAIT;
  DATA_MODE // Запись данных
};

void ST7735S_SPI::setAddr(byte x0, byte y0, byte x1, byte y1)
{
  DISPLAY_CONNECT; // CS Выбор дисплея

  sendCommand(CASET); // Column Address Set
  SPDR = 0;
  sendByte(x0);
  SPI_WAIT;
  SPDR = 0;
  sendByte(x1);

  sendCommand(RASET); // Row Address Set
  SPDR = 0;
  sendByte(y0);
  SPI_WAIT;
  SPDR = 0;
  sendByte(y1);

  sendCommand(RAMWR); // Memory Write
};

void ST7735S_SPI::sendByte(byte data)
{
  SPI_WAIT;
  SPDR = data;
};

#if RGB_FORMAT == RGB_12
void ST7735S_SPI::sendRGB(uint16_t color)
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
void ST7735S_SPI::sendRGB(uint16_t data)
{
  SPI_WAIT;
  SPDR = data >> 8;
  asm volatile("nop");
  SPI_WAIT;
  SPDR = data;
};

#elif RGB_FORMAT == RGB_18
void ST7735S_SPI::sendRGB(uint16_t data) // формат 0x0rgb
{
  sendRGB((data >> 4) & 0xf0, data & 0xf0, data << 4);
}
#endif

void ST7735S_SPI::sendRGB(uint32_t color)
{
  sendRGB(color >> 16, color >> 8, color);
}

void ST7735S_SPI::sendRGB(RGB color)
{
#if RGB_FORMAT == RGB_12 || RGB_FORMAT == RGB_16
  sendRGB((uint16_t)color);
#elif RGB_FORMAT == RGB_18
  sendRGB(color.red, color.green, color.blue);
#endif
}

void ST7735S_SPI::sendRGB(byte r, byte g, byte b)
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
  word len = (x1 - x0 + 1) * (y1 - y0 + 1);
  setAddr(x0, y0, x1, y1);

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
