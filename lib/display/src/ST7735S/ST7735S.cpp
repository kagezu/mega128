#include <avr/pgmspace.h>
#include "ST7735S.h"

#if !LCD_SPI

ST7735S::ST7735S()
{
  INIT_LCD;
  delayMicroseconds(1000); // Ждать стабилизации напряжений
  DISPLAY_CONNECT;          // CS Выбор дисплея

  send_command(SLPOUT);      // Проснуться

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

void ST7735S::send_command(byte command)
{
  COMMAND_MODE; // Запись команды
  send_byte(command);
  DATA_MODE // Запись данных
};

void ST7735S::set_addr(byte x0, byte y0, byte x1, byte y1)
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

void ST7735S::send_zero()
{
  LCD_PORT &= ~LCD_SDA;

  byte res = LCD_PORT & ~LCD_SCK;
  byte set = LCD_PORT | LCD_SCK;

  LCD_PORT = res;
  LCD_PORT = set;
  LCD_PORT = res;
  LCD_PORT = set;
  LCD_PORT = res;
  LCD_PORT = set;
  LCD_PORT = res;
  LCD_PORT = set;
  LCD_PORT = res;
  LCD_PORT = set;
  LCD_PORT = res;
  LCD_PORT = set;
  LCD_PORT = res;
  LCD_PORT = set;
  LCD_PORT = res;
  LCD_PORT = set;
};

void ST7735S::send_byte(byte data)
{
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
};

#if RGB_FORMAT == RGB_12
void ST7735S::send_rgb(uint16_t data)
{
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
};

#elif RGB_FORMAT == RGB_16
void ST7735S::send_rgb(uint16_t data)
{
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
};

#elif RGB_FORMAT == RGB_18
void ST7735S::send_rgb(uint16_t data) // формат 0x0rgb
{
  send_rgb((data >> 4) & 0xf0, data & 0xf0, data << 4);
}
#endif

void ST7735S::send_rgb(uint32_t color)
{
  send_rgb(color >> 16, color >> 8, color);
}

void ST7735S::send_rgb(RGB color)
{
#if RGB_FORMAT == RGB_12 || RGB_FORMAT == RGB_16
  send_rgb((uint16_t)color);
#elif RGB_FORMAT == RGB_18
  send_rgb(color.red, color.green, color.blue);
#endif
}

void ST7735S::send_rgb(byte r, byte g, byte b)
{
  byte b0 = LCD_PORT & ~(LCD_SDA | LCD_SCK);
  byte b1 = (LCD_PORT | LCD_SDA) & ~LCD_SCK;
  byte set = LCD_PORT;

  LCD_PORT = b & 0x80 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = b & 0x40 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = b & 0x20 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = b & 0x10 ? b1 : b0;
  LCD_PORT = set;
#if RGB_FORMAT == RGB_18 || RGB_FORMAT == RGB_16
  LCD_PORT = b & 0x8 ? b1 : b0;
  LCD_PORT = set;
#endif
#if RGB_FORMAT == RGB_18
  LCD_PORT = b & 0x4 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = b0;
  LCD_PORT = set;
  LCD_PORT = b0;
  LCD_PORT = set;
#endif

  LCD_PORT = g & 0x80 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = g & 0x40 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = g & 0x20 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = g & 0x10 ? b1 : b0;
  LCD_PORT = set;
#if RGB_FORMAT == RGB_18 || RGB_FORMAT == RGB_16
  LCD_PORT = g & 0x8 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = g & 0x4 ? b1 : b0;
  LCD_PORT = set;
#endif
#if RGB_FORMAT == RGB_18
  LCD_PORT = b0;
  LCD_PORT = set;
  LCD_PORT = b0;
  LCD_PORT = set;
#endif

  LCD_PORT = r & 0x80 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = r & 0x40 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = r & 0x20 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = r & 0x10 ? b1 : b0;
  LCD_PORT = set;
#if RGB_FORMAT == RGB_18 || RGB_FORMAT == RGB_16
  LCD_PORT = r & 0x8 ? b1 : b0;
  LCD_PORT = set;
#endif
#if RGB_FORMAT == RGB_18
  LCD_PORT = r & 0x4 ? b1 : b0;
  LCD_PORT = set;
  LCD_PORT = b0;
  LCD_PORT = set;
  LCD_PORT = b0;
  LCD_PORT = set;
#endif
};

void ST7735S::rect(byte x0, byte y0, byte x1, byte y1, RGB color)
{
#if RGB_FORMAT == RGB_12
  byte r = (uint16_t)color << 4;
  byte g = (uint16_t)color;
  byte b = (uint16_t)color >> 4;
#elif RGB_FORMAT == RGB_16
  byte r = (uint16_t)color << 3;
  byte g = (uint16_t)color >> 3;
  byte b = (uint16_t)color >> 8;
#elif RGB_FORMAT == RGB_18
  byte r = color.red;
  byte g = color.green;
  byte b = color.blue;
#endif

  DISPLAY_CONNECT;
  set_addr(x0, y0, x1, y1);
  word len = (x1 - x0 + 1) * (y1 - y0 + 1);
  byte b0 = LCD_PORT & ~(LCD_SDA | LCD_SCK);
  byte b1 = (LCD_PORT | LCD_SDA) & ~LCD_SCK;
  byte set = LCD_PORT;

  while (len--) {
    LCD_PORT = b & 0x80 ? b1 : b0;
    LCD_PORT = set;
    LCD_PORT = b & 0x40 ? b1 : b0;
    LCD_PORT = set;
    LCD_PORT = b & 0x20 ? b1 : b0;
    LCD_PORT = set;
    LCD_PORT = b & 0x10 ? b1 : b0;
    LCD_PORT = set;
  #if RGB_FORMAT == RGB_18 || RGB_FORMAT == RGB_16
    LCD_PORT = b & 0x8 ? b1 : b0;
    LCD_PORT = set;
  #endif
  #if RGB_FORMAT == RGB_18
    LCD_PORT = b & 0x4 ? b1 : b0;
    LCD_PORT = set;
    LCD_PORT = b0;
    LCD_PORT = set;
    LCD_PORT = b0;
    LCD_PORT = set;
  #endif

    LCD_PORT = g & 0x80 ? b1 : b0;
    LCD_PORT = set;
    LCD_PORT = g & 0x40 ? b1 : b0;
    LCD_PORT = set;
    LCD_PORT = g & 0x20 ? b1 : b0;
    LCD_PORT = set;
    LCD_PORT = g & 0x10 ? b1 : b0;
    LCD_PORT = set;
  #if RGB_FORMAT == RGB_18 || RGB_FORMAT == RGB_16
    LCD_PORT = g & 0x8 ? b1 : b0;
    LCD_PORT = set;
    LCD_PORT = g & 0x4 ? b1 : b0;
    LCD_PORT = set;
  #endif
  #if RGB_FORMAT == RGB_18
    LCD_PORT = b0;
    LCD_PORT = set;
    LCD_PORT = b0;
    LCD_PORT = set;
  #endif

    LCD_PORT = r & 0x80 ? b1 : b0;
    LCD_PORT = set;
    LCD_PORT = r & 0x40 ? b1 : b0;
    LCD_PORT = set;
    LCD_PORT = r & 0x20 ? b1 : b0;
    LCD_PORT = set;
    LCD_PORT = r & 0x10 ? b1 : b0;
    LCD_PORT = set;
  #if RGB_FORMAT == RGB_18 || RGB_FORMAT == RGB_16
    LCD_PORT = r & 0x8 ? b1 : b0;
    LCD_PORT = set;
  #endif
  #if RGB_FORMAT == RGB_18
    LCD_PORT = r & 0x4 ? b1 : b0;
    LCD_PORT = set;
    LCD_PORT = b0;
    LCD_PORT = set;
    LCD_PORT = b0;
    LCD_PORT = set;
  #endif
  }
  DISPLAY_DISCONNECT;
};

#endif
