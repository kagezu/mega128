#include <Arduino.h>

#define SET_BIT(port, mask) port |= mask;
#define RES_BIT(port, mask) port &= ~mask;

#define LCD_CS _BV(PB6)
#define LCD_RESET _BV(PE5)
#define LCD_RS _BV(PE4)
#define LCD_SDA _BV(PE3)
#define LCD_SCK _BV(PE2)

#define LCD_PORT PORTE

#define INIT_LCD_PORT                              \
  DDRE |= LCD_RS | LCD_RESET | LCD_SDA | LCD_SCK;  \
  PORTE |= LCD_RS | LCD_RESET | LCD_SDA | LCD_SCK; \
  DDRB |= LCD_CS;                                  \
  PORTB |= LCD_CS;

#define SET_CS SET_BIT(PORTB, LCD_CS)
#define SET_RS SET_BIT(LCD_PORT, LCD_RS)
#define SET_RESET SET_BIT(LCD_PORT, LCD_RESET)
#define SET_SDA SET_BIT(LCD_PORT, LCD_SDA)
#define SET_SCK SET_BIT(LCD_PORT, LCD_SCK)

#define RES_CS RES_BIT(PORTB, LCD_CS)
#define RES_RS RES_BIT(LCD_PORT, LCD_RS)
#define RES_RESET RES_BIT(LCD_PORT, LCD_RESET)
#define RES_SDA RES_BIT(LCD_PORT, LCD_SDA)
#define RES_SCK RES_BIT(LCD_PORT, LCD_SCK)

class ST7735S
{
public:
  word color = 0;

  ST7735S() {};

  void reset()
  {
    SET_RESET
    delayMicroseconds(55000);
    RES_RESET
    delayMicroseconds(55000);
    SET_RESET
    delayMicroseconds(55000);
  };

  void data_0()
  {
    LCD_PORT &= ~LCD_SDA;

    RES_SCK
    SET_SCK
    RES_SCK
    SET_SCK
    RES_SCK
    SET_SCK
    RES_SCK
    SET_SCK
    RES_SCK
    SET_SCK
    RES_SCK
    SET_SCK
    RES_SCK
    SET_SCK
    RES_SCK
    SET_SCK
  };

  void data_rgb(unsigned char r, unsigned char g, unsigned char b)
  {
    register byte bit0 = LCD_PORT & ~(LCD_SDA | LCD_SCK);
    register byte bit1 = (LCD_PORT | LCD_SDA) & ~LCD_SCK;
    register byte set = LCD_PORT;

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

  void data_12(word c)
  {
    register byte bit0 = LCD_PORT & ~(LCD_SDA | LCD_SCK);
    register byte bit1 = (LCD_PORT | LCD_SDA) & ~LCD_SCK;
    register byte set = LCD_PORT; // | LCD_SCK;

    LCD_PORT = c & 0x800 ? bit1 : bit0;
    LCD_PORT = set;
    LCD_PORT = c & 0x400 ? bit1 : bit0;
    LCD_PORT = set;
    LCD_PORT = c & 0x200 ? bit1 : bit0;
    LCD_PORT = set;
    LCD_PORT = c & 0x100 ? bit1 : bit0;
    LCD_PORT = set;

    LCD_PORT = c & 0x80 ? bit1 : bit0;
    LCD_PORT = set;
    LCD_PORT = c & 0x40 ? bit1 : bit0;
    LCD_PORT = set;
    LCD_PORT = c & 0x20 ? bit1 : bit0;
    LCD_PORT = set;
    LCD_PORT = c & 0x10 ? bit1 : bit0;
    LCD_PORT = set;

    LCD_PORT = c & 0x8 ? bit1 : bit0;
    LCD_PORT = set;
    LCD_PORT = c & 0x4 ? bit1 : bit0;
    LCD_PORT = set;
    LCD_PORT = c & 0x2 ? bit1 : bit0;
    LCD_PORT = set;
    LCD_PORT = c & 0x1 ? bit1 : bit0;
    LCD_PORT = set;
  };

  void data_8(unsigned char d)
  {
    register byte bit0 = LCD_PORT & ~(LCD_SDA | LCD_SCK);
    register byte bit1 = (LCD_PORT | LCD_SDA) & ~LCD_SCK;
    register byte set = LCD_PORT;

    LCD_PORT = d & 0x80 ? bit1 : bit0;
    LCD_PORT = set;
    LCD_PORT = d & 0x40 ? bit1 : bit0;
    LCD_PORT = set;
    LCD_PORT = d & 0x20 ? bit1 : bit0;
    LCD_PORT = set;
    LCD_PORT = d & 0x10 ? bit1 : bit0;
    LCD_PORT = set;

    LCD_PORT = d & 0x8 ? bit1 : bit0;
    LCD_PORT = set;
    LCD_PORT = d & 0x4 ? bit1 : bit0;
    LCD_PORT = set;
    LCD_PORT = d & 0x2 ? bit1 : bit0;
    LCD_PORT = set;
    LCD_PORT = d & 0x1 ? bit1 : bit0;
    LCD_PORT = set;
  };

  void com(unsigned char d)
  {
    RES_RS // Запись команды
        data_8(d);
    SET_RS // Запись данных
  };

  void init()
  {
    INIT_LCD_PORT
    reset();
    RES_CS         // CS Выбор дисплея
        com(0x11); // Проснуться

    delayMicroseconds(55000); // Ждать стабилизации напряжений

    com(0xB1); // ?
    data_8(0x05);
    data_8(0x3C);
    data_8(0x3C);

    com(0xB2); // ?
    data_8(0x05);
    data_8(0X3C);
    data_8(0X3C);

    com(0xB3); // ?
    data_8(0x05);
    data_8(0x3C);
    data_8(0x3C);
    data_8(0x05);
    data_8(0x3C);
    data_8(0x3C);

    com(0xB4); // ?
    data_8(0x03);

    com(0xC0); // ?
    data_8(0x28);
    data_8(0x08);
    data_8(0x04);

    com(0xC1); // ?
    data_8(0xC0);

    com(0xC2); // ?
    data_8(0x0D);
    data_8(0X00);

    com(0xC3); // ?
    data_8(0x8D);
    data_8(0x2A);

    com(0xC4); // ?
    data_8(0x8D);
    data_8(0xEE);

    com(0xC5); // ?
    data_8(0x1A);

    com(0x17); // ?
    data_8(0x05);

    com(0x36); // Memory Data Access Control
    data_8(0xD8);

    com(0xE0); // ?
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

    com(0xE1); // ?
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

    com(0x3A); // Interface Pixel Format
    data_8(0x03);

    com(0x29); // Display On

    SET_CS // CS Снять выбор дисплея
  };

  void addr(byte x1, byte y1, byte x2, byte y2)
  {
    com(0x2a); // Column Address Set
    data_0();
    data_8(x1);
    data_0();
    data_8(x2);

    com(0x2b); // Row Address Set
    data_0();
    data_8(y1);
    data_0();
    data_8(y2);

    com(0x2c); // Memory Write
  };

  void rect(byte x1, byte y1, byte x2, byte y2, unsigned int c)
  {

    RES_CS // CS Выбор дисплея

        addr(x1, y1, x2, y2);

    SET_RS // Запись данных

        register word len = (x2 - x1 + 1) * (y2 - y1 + 1) + 1;

    register byte b0 = LCD_PORT & ~(LCD_SDA | LCD_SCK);
    register byte b1 = (LCD_PORT | LCD_SDA) & ~LCD_SCK;
    register byte set = LCD_PORT;

    while (len--)
    {

      LCD_PORT = c & 0x800 ? b1 : b0;
      LCD_PORT = set;
      LCD_PORT = c & 0x400 ? b1 : b0;
      LCD_PORT = set;
      LCD_PORT = c & 0x200 ? b1 : b0;
      LCD_PORT = set;
      LCD_PORT = c & 0x100 ? b1 : b0;
      LCD_PORT = set;

      LCD_PORT = c & 0x80 ? b1 : b0;
      LCD_PORT = set;
      LCD_PORT = c & 0x40 ? b1 : b0;
      LCD_PORT = set;
      LCD_PORT = c & 0x20 ? b1 : b0;
      LCD_PORT = set;
      LCD_PORT = c & 0x10 ? b1 : b0;
      LCD_PORT = set;

      LCD_PORT = c & 0x8 ? b1 : b0;
      LCD_PORT = set;
      LCD_PORT = c & 0x4 ? b1 : b0;
      LCD_PORT = set;
      LCD_PORT = c & 0x2 ? b1 : b0;
      LCD_PORT = set;
      LCD_PORT = c & 0x1 ? b1 : b0;
      LCD_PORT = set;
    }

    SET_CS // CS Снять выбор дисплея
  };

  void pixel(byte x, byte y, word c)
  {
    addr(x, y, x, y);

    RES_SCK
    SET_SCK
    RES_SCK
    SET_SCK
    RES_SCK
    SET_SCK
    RES_SCK
    SET_SCK
    RES_SCK
    SET_SCK
    RES_SCK
    SET_SCK
    RES_SCK
    SET_SCK
    RES_SCK
    SET_SCK
    RES_SCK
    SET_SCK
    RES_SCK
    SET_SCK
    RES_SCK
    SET_SCK
    RES_SCK
    SET_SCK

    data_12(c);
  }

  void test(byte d)
  {

    RES_CS // CS Выбор дисплея

        addr(0, 0, 127, 159);

    SET_RS // Запись данных

        register byte bit0 = LCD_PORT & ~(LCD_SDA | LCD_SCK);
    register byte bit1 = (LCD_PORT | LCD_SDA) & ~LCD_SCK;
    register byte set = LCD_PORT;

    register word r, g, b;

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
        g = ((yy - xx) >> 8) + d >> 1;
        b = ((xx - yy) >> 8) + d << 1;
        //
        data_rgb((r), (g), (b));
        /*
             LCD_PORT = b&0x80 ? bit1 : bit0;
             LCD_PORT = set;
             LCD_PORT = b&0x40 ? bit1 : bit0;
             LCD_PORT = set;
             LCD_PORT = b&0x20 ? bit1 : bit0;
             LCD_PORT = set;
             LCD_PORT = b&0x10 ? bit1 : bit0;
             LCD_PORT = set;
             /
             LCD_PORT = b&0x8 ? bit1 : bit0;
             LCD_PORT = set;
             LCD_PORT = b&0x4 ? bit1 : bit0;
             LCD_PORT = set;
             LCD_PORT = b&0x2 ? bit1 : bit0;
             LCD_PORT = set;
             LCD_PORT = b&0x1 ? bit1 : bit0;
             LCD_PORT = set;
         /*
             LCD_PORT = g&0x80 ? bit1 : bit0;
             LCD_PORT = set;
             LCD_PORT = g&0x40 ? bit1 : bit0;
             LCD_PORT = set;
             LCD_PORT = g&0x20 ? bit1 : bit0;
             LCD_PORT = set;
             LCD_PORT = g&0x10 ? bit1 : bit0;
             LCD_PORT = set;
             /
             LCD_PORT = g&0x8 ? bit1 : bit0;
             LCD_PORT = set;
             LCD_PORT = g&0x4 ? bit1 : bit0;
             LCD_PORT = set;
             LCD_PORT = g&0x2 ? bit1 : bit0;
             LCD_PORT = set;
             LCD_PORT = g&0x1 ? bit1 : bit0;
             LCD_PORT = set;
         /*
             LCD_PORT = r&0x80 ? bit1 : bit0;
             LCD_PORT = set;
             LCD_PORT = r&0x40 ? bit1 : bit0;
             LCD_PORT = set;
             LCD_PORT = r&0x20 ? bit1 : bit0;
             LCD_PORT = set;
             LCD_PORT = r&0x10 ? bit1 : bit0;
             LCD_PORT = set;
             /
             LCD_PORT = r&0x8 ? bit1 : bit0;
             LCD_PORT = set;
             LCD_PORT = r&0x4 ? bit1 : bit0;
             LCD_PORT = set;
             LCD_PORT = r&0x2 ? bit1 : bit0;
             LCD_PORT = set;
             LCD_PORT = r&0x1 ? bit1 : bit0;
             LCD_PORT = set;
        */
      }
    }

    SET_CS // CS Снять выбор дисплея
  };

  void clear(unsigned int c)
  {
    rect(0, 0, 127, 159, c);
  };
};
