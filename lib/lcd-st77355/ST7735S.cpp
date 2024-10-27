#include "ST7735S.h"
#include "font_6x5.h"

ST7735S::ST7735S()
{
  INIT_LCD_PORT
  delayMicroseconds(15000); // Ждать стабилизации напряжений
  DISPLAY_CONNECT;          // CS Выбор дисплея

  command(SLPOUT);          // Проснуться
  delayMicroseconds(15000); // Ждать стабилизации напряжений

  command(FRMCTR1); // In normal mode (Full colors)
  data_8(0x05);
  data_8(0x3C);
  data_8(0x3C);

  command(FRMCTR2); // In Idle mode (8-colors)
  data_8(0x05);
  data_8(0X3C);
  data_8(0X3C);

  command(FRMCTR3); // In partial mode + Full colors
  data_8(0x05);
  data_8(0x3C);
  data_8(0x3C);
  data_8(0x05);
  data_8(0x3C);
  data_8(0x3C);

  command(INVCTR); // Display inversion control
  data_8(0x03);

  command(PWCTR1); // Power control setting
  data_8(0x28);
  data_8(0x08);
  data_8(0x04);

  command(PWCTR2); // Power control setting
  data_8(0xC0);

  command(PWCTR3); // In normal mode (Full colors)
  data_8(0x0D);
  data_8(0X00);

  command(PWCTR4); // In Idle mode (8-colors)
  data_8(0x8D);
  data_8(0x2A);

  command(PWCTR5); // In partial mode + Full colors
  data_8(0x8D);
  data_8(0xEE);

  command(VMCTR1); // VCOM control 1
  data_8(0x1A);

  command(0x17); // ?
  data_8(0x05);

  command(MADCTL); // Memory Data Access Control
  data_8(0xD8);

  command(GAMCTRP1); // Set Gamma adjustment (+ polarity)
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

  command(GAMCTRN1); // Set Gamma adjustment (- polarity)
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

  command(COLMOD);
  data_8(RGB_FORMAT);

  command(DISPON); // Display On
  DISPLAY_DISCONNECT
}

void ST7735S::command(byte command)
{
  COMMAND_MODE; // Запись команды
  data_8(command);
  DATA_MODE // Запись данных
};

void ST7735S::set_rect(byte x1, byte y1, byte x2, byte y2)
{
  DISPLAY_CONNECT; // CS Выбор дисплея

  command(CASET); // Column Address Set
  data_0();
  data_8(x1);
  data_0();
  data_8(x2);

  command(RASET); // Row Address Set
  data_0();
  data_8(y1);
  data_0();
  data_8(y2);

  command(RAMWR); // Memory Write
};

void ST7735S::data_0()
{
  LCD_PORT &= ~LCD_SDA;
  TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK
};

void ST7735S::data_8(byte data)
{
  CLI;
  byte b0 = LCD_PORT & ~(LCD_SDA | LCD_SCK);
  byte b1 = (LCD_PORT | LCD_SDA) & ~LCD_SCK;
  byte set = LCD_PORT;

  LCD_PORT = data & 0x80 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x40 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x20 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x10 ? b1 : b0;
  LCD_PORT = set;

  LCD_PORT = data & 0x8 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x4 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x2 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x1 ? b1 : b0;
  LCD_PORT = set;

  SEI;
};

#if RGB_FORMAT == RGB_12

void ST7735S::data_12(word data)
{
  CLI;
  byte b0 = LCD_PORT & ~(LCD_SDA | LCD_SCK);
  byte b1 = (LCD_PORT | LCD_SDA) & ~LCD_SCK;
  byte set = LCD_PORT;

  LCD_PORT = data & 0x800 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x400 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x200 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x100 ? b1 : b0;
  LCD_PORT = set;

  LCD_PORT = data & 0x80 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x40 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x20 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x10 ? b1 : b0;
  LCD_PORT = set;

  LCD_PORT = data & 0x8 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x4 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x2 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x1 ? b1 : b0;
  LCD_PORT = set;

  SEI;
};

void ST7735S::data_12(byte r, byte g, byte b)
{
  CLI;
  byte b0 = LCD_PORT & ~(LCD_SDA | LCD_SCK);
  byte b1 = (LCD_PORT | LCD_SDA) & ~LCD_SCK;
  byte set = LCD_PORT;

  LCD_PORT = b & 0x8 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = b & 0x4 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = b & 0x2 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = b & 0x1 ? b1 : b0;
  LCD_PORT = set;

  LCD_PORT = g & 0x8 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = g & 0x4 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = g & 0x2 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = g & 0x1 ? b1 : b0;
  LCD_PORT = set;

  LCD_PORT = r & 0x8 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = r & 0x4 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = r & 0x2 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = r & 0x1 ? b1 : b0;
  LCD_PORT = set;

  SEI;
};

#elif RGB_FORMAT == RGB_16

void ST7735S::data_16(word data)
{
  CLI;
  byte b0 = LCD_PORT & ~(LCD_SDA | LCD_SCK);
  byte b1 = (LCD_PORT | LCD_SDA) & ~LCD_SCK;
  byte set = LCD_PORT;

  LCD_PORT = data & 0x8000 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x4000 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x2000 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x1000 ? b1 : b0;
  LCD_PORT = set;

  LCD_PORT = data & 0x800 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x400 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x200 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x100 ? b1 : b0;
  LCD_PORT = set;

  LCD_PORT = data & 0x80 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x40 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x20 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x10 ? b1 : b0;
  LCD_PORT = set;

  LCD_PORT = data & 0x8 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x4 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x2 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = data & 0x1 ? b1 : b0;
  LCD_PORT = set;

  SEI;
};
void ST7735S::data_16(byte r, byte g, byte b)
{
  CLI;
  byte b0 = LCD_PORT & ~(LCD_SDA | LCD_SCK);
  byte b1 = (LCD_PORT | LCD_SDA) & ~LCD_SCK;
  byte set = LCD_PORT;

  LCD_PORT = r & 0x10 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = r & 0x8 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = r & 0x4 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = r & 0x2 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = r & 0x1 ? b1 : b0;
  LCD_PORT = set;

  LCD_PORT = g & 0x20 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = g & 0x10 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = g & 0x8 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = g & 0x4 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = g & 0x2 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = g & 0x1 ? b1 : b0;
  LCD_PORT = set;

  LCD_PORT = b & 0x10 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = b & 0x8 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = b & 0x4 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = b & 0x2 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = b & 0x1 ? b1 : b0;
  LCD_PORT = set;

  SEI;
};

#elif RGB_FORMAT == RGB_18

void ST7735S::data_24(byte r, byte g, byte b)
{
  CLI;
  byte b0 = LCD_PORT & ~(LCD_SDA | LCD_SCK);
  byte b1 = (LCD_PORT | LCD_SDA) & ~LCD_SCK;
  byte set = LCD_PORT;

  LCD_PORT = r & 0x20 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = r & 0x10 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = r & 0x8 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = r & 0x4 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = r & 0x2 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = r & 0x1 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = b0;
  LCD_PORT = set;
  LCD_PORT = b0;
  LCD_PORT = set;

  LCD_PORT = g & 0x20 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = g & 0x10 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = g & 0x8 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = g & 0x4 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = g & 0x2 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = g & 0x1 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = b0;
  LCD_PORT = set;
  LCD_PORT = b0;
  LCD_PORT = set;

  LCD_PORT = b & 0x20 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = b & 0x10 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = b & 0x8 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = b & 0x4 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = b & 0x2 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = b & 0x1 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = b0;
  LCD_PORT = set;
  LCD_PORT = b0;
  LCD_PORT = set;

  SEI;
};

#endif

inline void ST7735S::data_rgb(byte r, byte g, byte b)
{
#if RGB_FORMAT == RGB_12
  data_12(r, g, b);
#elif RGB_FORMAT == RGB_16
  data_16(r, g, b);
#elif RGB_FORMAT == RGB_18
  data_24(r, g, b);
#endif
};

void ST7735S::pixel(byte x, byte y, word color)
{
  set_rect(x, y, x, y);
  TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK;

#if RGB_FORMAT == RGB_12
  data_12(color);

#elif RGB_FORMAT == RGB_16
  TICK_TSK TICK_TSK TICK_TSK TICK_TSK;
  data_8(color >> 8);
  data_8(color);

#elif RGB_FORMAT == RGB_18 // Заглушка
  TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK;
  data_8(color);
  data_8(color);
  data_8(color);

#endif

  DISPLAY_DISCONNECT;
}

void ST7735S::pixel(byte x, byte y, byte r, byte g, byte b)
{
  set_rect(x, y, x, y);
  TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK;

#if RGB_FORMAT == RGB_12
  data_12(r, g, b);

#elif RGB_FORMAT == RGB_16
  TICK_TSK TICK_TSK TICK_TSK TICK_TSK;
  data_16(r, g, b);

#elif RGB_FORMAT == RGB_18
  TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK TICK_TSK;
  data_24(r, g, b);

#endif

  DISPLAY_DISCONNECT;
}

void ST7735S::rect(byte x1, byte y1, byte x2, byte y2, word color)
{
  set_rect(x1, y1, x2, y2);
  word len = (x2 - x1 + 1) * (y2 - y1 + 1) + 1;

  CLI;
  byte b0 = LCD_PORT & ~(LCD_SDA | LCD_SCK);
  byte b1 = (LCD_PORT | LCD_SDA) & ~LCD_SCK;
  byte set = LCD_PORT;

  while (len--)
  {
#if RGB_FORMAT == RGB_16
    LCD_PORT = color & 0x8000 ? b1 : b0;
    LCD_PORT = set;
    LCD_PORT = color & 0x4000 ? b1 : b0;
    LCD_PORT = set;
    LCD_PORT = color & 0x2000 ? b1 : b0;
    LCD_PORT = set;
    LCD_PORT = color & 0x1000 ? b1 : b0;
    LCD_PORT = set;

#endif

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

  SEI;
  DISPLAY_DISCONNECT // CS Снять выбор дисплея
};

void ST7735S::symbol(byte symbol, byte x, byte y, byte dx, byte dy)
{
  set_rect(x, y, x + dx, y + dy);
  for (byte j = 0; j <= dy; y++)
  {
    byte data = 0xaa; // pgm_read_byte(font_6x5 + symbol * 6 - 192 + j);
    for (byte x = 0; x <= dx; x++)
      if (data & (1 << x))
        data_16(0xff, 0xff, 0xff);
      else
        data_16(0x00, 0x00, 0x00);
  }
  DISPLAY_DISCONNECT
}

// тестирование дисплея

void ST7735S::test(byte d)
{
  set_rect(0, 0, MAX_X, MAX_Y);
  for (byte y = 0; y <= MAX_Y; y++)
  {
    word yy = y * y;

    for (byte x = 0; x <= MAX_X; x++)
    {
      word xx = x * x;
      word r = ((xx + yy) >> 8) + d;
      word g = ((yy - xx) >> 8) + d;
      word b = ((x * y) >> 8) + d;

      data_rgb(r, g, b);
    }
  }
  DISPLAY_DISCONNECT
}

void ST7735S::write_mem(byte d)
{
  word *ptr = (word *)0x6000;
  // set_rect(0, 0, MAX_X, MAX_Y);
  for (byte y = 0; y <= MAX_Y; y++)
  {
    word yy = y * y;

    for (byte x = 0; x <= MAX_X; x++)
    {
      word xx = x * x;
      word r = ((xx + yy) >> 8) + d;
      word g = ((yy - xx) >> 8) + d;
      word b = ((x * y) >> 8) + d;

      // data_rgb(r, g, b);
      *ptr = (r << 11) + ((g & 0x3f) << 5) + (b & 0x001f);
      ptr++;
    }
  }

  // DISPLAY_DISCONNECT // CS Снять выбор дисплея
}

void ST7735S::read_mem()
{
  word *ptr = (word *)0x6000;
  set_rect(0, 0, MAX_X, MAX_Y);
  word count = 160 * 128;
  while (count--)
    data_16(*(ptr++));

  DISPLAY_DISCONNECT // CS Снять выбор дисплея
}
