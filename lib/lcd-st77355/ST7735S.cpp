#include "ST7735S.h"

ST7735S::ST7735S(byte format)
{
  INIT_LCD_PORT
  SET_RESET
  // delayMicroseconds(15000);
  // RES_RESET
  // delayMicroseconds(15000);
  // SET_RESET
  // delayMicroseconds(15000);
  RES_CS; // CS Выбор дисплея

  command(0x11);            // Проснуться
  delayMicroseconds(15000); // Ждать стабилизации напряжений

  command(0xB1); // ?
  data_8(0x05);
  data_8(0x3C);
  data_8(0x3C);

  command(0xB2); // ?
  data_8(0x05);
  data_8(0X3C);
  data_8(0X3C);

  command(0xB3); // ?
  data_8(0x05);
  data_8(0x3C);
  data_8(0x3C);
  data_8(0x05);
  data_8(0x3C);
  data_8(0x3C);

  command(0xB4); // ?
  data_8(0x03);

  command(0xC0); // ?
  data_8(0x28);
  data_8(0x08);
  data_8(0x04);

  command(0xC1); // ?
  data_8(0xC0);

  command(0xC2); // ?
  data_8(0x0D);
  data_8(0X00);

  command(0xC3); // ?
  data_8(0x8D);
  data_8(0x2A);

  command(0xC4); // ?
  data_8(0x8D);
  data_8(0xEE);

  command(0xC5); // ?
  data_8(0x1A);

  command(0x17); // ?
  data_8(0x05);

  command(0x36); // Memory Data Access Control
  data_8(0xD8);

  command(0xE0); // ?
  data_8(0x03);
  data_8(0x22);
  data_8(0x07);
  data_8(0x0A);
  data_8(0x2E);
  data_8(0x30);
  data_8(0x25);
  data_8(0x2A);
  data_8(0x28);
  data_8(0x26);
  data_8(0x2E);
  data_8(0x3A);
  data_8(0x00);
  data_8(0x01);
  data_8(0x03);
  data_8(0x13);

  command(0xE1); // ?
  data_8(0x04);
  data_8(0x16);
  data_8(0x06);
  data_8(0x0D);
  data_8(0x2D);
  data_8(0x26);
  data_8(0x23);
  data_8(0x27);
  data_8(0x27);
  data_8(0x25);
  data_8(0x2D);
  data_8(0x3B);
  data_8(0x00);
  data_8(0x01);
  data_8(0x04);
  data_8(0x13);

  command(0x3A); // Interface Pixel Format
  data_8(format);

  command(0x29); // Display On

  SET_CS // CS Снять выбор дисплея
}

void ST7735S::setBlock(byte x1, byte y1, byte x2, byte y2)
{
  RES_CS; // CS Выбор дисплея

  command(0x2a); // Column Address Set
  data_0();
  data_8(x1);
  data_0();
  data_8(x2);

  command(0x2b); // Row Address Set
  data_0();
  data_8(y1);
  data_0();
  data_8(y2);

  command(0x2c); // Memory Write
};

void ST7735S::data_12(word data)
{
  byte bit0 = LCD_PORT & ~(LCD_SDA | LCD_SCK);
  byte bit1 = (LCD_PORT | LCD_SDA) & ~LCD_SCK;
  byte set = LCD_PORT;

  LCD_PORT = data & 0x800 ? bit1 : bit0;
  LCD_PORT = set;
  LCD_PORT = data & 0x400 ? bit1 : bit0;
  LCD_PORT = set;
  LCD_PORT = data & 0x200 ? bit1 : bit0;
  LCD_PORT = set;
  LCD_PORT = data & 0x100 ? bit1 : bit0;
  LCD_PORT = set;

  LCD_PORT = data & 0x80 ? bit1 : bit0;
  LCD_PORT = set;
  LCD_PORT = data & 0x40 ? bit1 : bit0;
  LCD_PORT = set;
  LCD_PORT = data & 0x20 ? bit1 : bit0;
  LCD_PORT = set;
  LCD_PORT = data & 0x10 ? bit1 : bit0;
  LCD_PORT = set;

  LCD_PORT = data & 0x8 ? bit1 : bit0;
  LCD_PORT = set;
  LCD_PORT = data & 0x4 ? bit1 : bit0;
  LCD_PORT = set;
  LCD_PORT = data & 0x2 ? bit1 : bit0;
  LCD_PORT = set;
  LCD_PORT = data & 0x1 ? bit1 : bit0;
  LCD_PORT = set;
};

void ST7735S::data_8(byte data)
{
  byte bit0 = LCD_PORT & ~(LCD_SDA | LCD_SCK);
  byte bit1 = (LCD_PORT | LCD_SDA) & ~LCD_SCK;
  byte set = LCD_PORT;

  LCD_PORT = data & 0x80 ? bit1 : bit0;
  LCD_PORT = set;
  LCD_PORT = data & 0x40 ? bit1 : bit0;
  LCD_PORT = set;
  LCD_PORT = data & 0x20 ? bit1 : bit0;
  LCD_PORT = set;
  LCD_PORT = data & 0x10 ? bit1 : bit0;
  LCD_PORT = set;

  LCD_PORT = data & 0x8 ? bit1 : bit0;
  LCD_PORT = set;
  LCD_PORT = data & 0x4 ? bit1 : bit0;
  LCD_PORT = set;
  LCD_PORT = data & 0x2 ? bit1 : bit0;
  LCD_PORT = set;
  LCD_PORT = data & 0x1 ? bit1 : bit0;
  LCD_PORT = set;
};

void ST7735S::pixel(byte x, byte y, word color)
{
  setBlock(x, y, x, y);
  TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK;
  data_12(color);
  SET_CS // CS Снять выбор дисплея
}

void ST7735S::rect(byte x1, byte y1, byte x2, byte y2, word color)
{
  setBlock(x1, y1, x2, y2);

  word len = (x2 - x1 + 1) * (y2 - y1 + 1) + 1;

  byte b0 = LCD_PORT & ~(LCD_SDA | LCD_SCK);
  byte b1 = (LCD_PORT | LCD_SDA) & ~LCD_SCK;
  byte set = LCD_PORT;

  while (len--)
  {

    LCD_PORT = color & 0x800 ? b1 : b0;
    LCD_PORT = set;
    LCD_PORT = color & 0x400 ? b1 : b0;
    LCD_PORT = set;
    LCD_PORT = color & 0x200 ? b1 : b0;
    LCD_PORT = set;
    LCD_PORT = color & 0x100 ? b1 : b0;
    LCD_PORT = set;

    LCD_PORT = color & 0x80 ? b1 : b0;
    LCD_PORT = set;
    LCD_PORT = color & 0x40 ? b1 : b0;
    LCD_PORT = set;
    LCD_PORT = color & 0x20 ? b1 : b0;
    LCD_PORT = set;
    LCD_PORT = color & 0x10 ? b1 : b0;
    LCD_PORT = set;

    LCD_PORT = color & 0x8 ? b1 : b0;
    LCD_PORT = set;
    LCD_PORT = color & 0x4 ? b1 : b0;
    LCD_PORT = set;
    LCD_PORT = color & 0x2 ? b1 : b0;
    LCD_PORT = set;
    LCD_PORT = color & 0x1 ? b1 : b0;
    LCD_PORT = set;
  }

  SET_CS // CS Снять выбор дисплея
};

void ST7735S::data_0()
{
  LCD_PORT &= ~LCD_SDA;
  TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK
};

void ST7735S::command(byte command)
{
  RES_RS // Запись команды
      data_8(command);
  SET_RS // Запись данных
};

void ST7735S::data_rgb(byte r, byte g, byte b)
{
  byte bit0 = LCD_PORT & ~(LCD_SDA | LCD_SCK);
  byte bit1 = (LCD_PORT | LCD_SDA) & ~LCD_SCK;
  byte set = LCD_PORT;

  LCD_PORT = b & 0x8 ? bit1 : bit0;
  LCD_PORT = set;
  LCD_PORT = b & 0x4 ? bit1 : bit0;
  LCD_PORT = set;
  LCD_PORT = b & 0x2 ? bit1 : bit0;
  LCD_PORT = set;
  LCD_PORT = b & 0x1 ? bit1 : bit0;
  LCD_PORT = set;

  LCD_PORT = g & 0x8 ? bit1 : bit0;
  LCD_PORT = set;
  LCD_PORT = g & 0x4 ? bit1 : bit0;
  LCD_PORT = set;
  LCD_PORT = g & 0x2 ? bit1 : bit0;
  LCD_PORT = set;
  LCD_PORT = g & 0x1 ? bit1 : bit0;
  LCD_PORT = set;

  LCD_PORT = r & 0x8 ? bit1 : bit0;
  LCD_PORT = set;
  LCD_PORT = r & 0x4 ? bit1 : bit0;
  LCD_PORT = set;
  LCD_PORT = r & 0x2 ? bit1 : bit0;
  LCD_PORT = set;
  LCD_PORT = r & 0x1 ? bit1 : bit0;
  LCD_PORT = set;
};

// тестирование дисплея

void ST7735S::test(byte d)
{
  setBlock(0, 0, 127, 159);

  word r, g, b;

  for (byte y = 0; y < 160; y++)
  {
    word yy = y * y;

    for (byte x = 0; x < 128; x++)
    {
      word xx = x * x;
      //       word sum = xx+yy;
      //       r = ((sum/(x+d))>>4)+d;
      //       g = ((sum/(y+d))>>4)+d>>1;
      //       b = ((sum/(x+y))>>4)+d<<1;

      r = ((xx + yy) >> 8) + d;
      g = ((yy - xx) >> 8) + (d >> 1);
      b = ((xx - yy) >> 8) + (d << 1);

      data_rgb(r, g, b);
    }
  }

  SET_CS // CS Снять выбор дисплея
};
