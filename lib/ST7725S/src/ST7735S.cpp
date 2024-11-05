#include <avr/pgmspace.h>
#include "ST7735S.h"

ST7735S::ST7735S()
{
  INIT_LCD_PORT
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

void ST7735S::sendCommand(byte command)
{
  COMMAND_MODE; // Запись команды
  sendByte(command);
  DATA_MODE // Запись данных
};

void ST7735S::setAddr(byte x1, byte y1, byte x2, byte y2)
{
  DISPLAY_CONNECT; // CS Выбор дисплея

  sendCommand(CASET); // Column Address Set
  sendZero();
  sendByte(x1);
  sendZero();
  sendByte(x2);

  sendCommand(RASET); // Row Address Set
  sendZero();
  sendByte(y1);
  sendZero();
  sendByte(y2);

  sendCommand(RAMWR); // Memory Write
};

void ST7735S::sendZero()
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

void ST7735S::sendByte(byte data)
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
void ST7735S::sendRGB(word data)
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
void ST7735S::sendRGB(word data)
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
#endif

void ST7735S::sendRGB(uint32_t color)
{
  sendRGB(color >> 16, color >> 8, color);
}

void ST7735S::sendRGB(byte r, byte g, byte b)
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

void ST7735S::pixel(byte x, byte y, word color)
{
  setAddr(x, y, x, y);

#if RGB_FORMAT == RGB_12
  sendRGB((word)0);
  sendRGB(color);

#elif RGB_FORMAT == RGB_16
  sendZero();
  sendZero();
  sendRGB(color);

#elif RGB_FORMAT == RGB_18 // Заглушка
  sendZero();
  sendZero();
  sendZero();
  sendRGB(color, color, color);

#endif

  DISPLAY_DISCONNECT;
}

void ST7735S::pixel(byte x, byte y, byte r, byte g, byte b)
{
  setAddr(x, y, x, y);

#if RGB_FORMAT == RGB_12
  sendRGB((word)0);
  sendRGB(r, g, b);

#elif RGB_FORMAT == RGB_16
  sendZero();
  sendZero();
  sendRGB(r, g, b);

#elif RGB_FORMAT == RGB_18
  sendZero();
  sendZero();
  sendZero();
  sendRGB(r, g, b);

#endif

  DISPLAY_DISCONNECT
}

void ST7735S::rect(byte x1, byte y1, byte x2, byte y2, uint32_t color)
{
  byte r = color >> 16;
  byte g = color >> 8;
  byte b = color;

  setAddr(x1, y1, x2, y2);
  word len = (x2 - x1 + 1) * (y2 - y1 + 1);
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
  #endif
  #if RGB_FORMAT == RGB_18
    LCD_PORT = g & 0x4 ? b1 : b0;
    LCD_PORT = set;
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

  DISPLAY_DISCONNECT
};

void ST7735S::symbol(const byte *font, byte symbol, byte x, byte y, byte dx, byte dy)
{
  // setAddr(x, y, x + dx - 1, y + dy - 1);
  setAddr(x, y, x + dx - 1, y + dy);
  for (char j = 0; j < dy; j++) {
    byte data = pgm_read_byte(font + symbol * 6 - 192 + j);
    for (byte i = 0; i < dx; i++)
      if (data & (1 << i))
        sendRGB(0xff, 0xff, 0xff);
      else
        sendRGB(0x00, 0x00, 0x00);
  }

  for (byte i = 0; i < dx; i++)
    sendRGB(0x00, 0x00, 0x00);

  DISPLAY_DISCONNECT
}

// тестирование дисплея

#define VIEWPORT_OFFSET 30
void ST7735S::demo(byte d)
{
  setAddr(0, 0, MAX_X, MAX_Y);
  for (byte y = VIEWPORT_OFFSET; y <= MAX_Y + VIEWPORT_OFFSET; y++) {
    word yy = y * y;

    for (byte x = VIEWPORT_OFFSET; x <= MAX_X + VIEWPORT_OFFSET; x++) {
      word xx = x * x;

      byte e = d << 2;
      word r = ((xx + yy) >> 6) + e;
      word g = ((yy - xx) >> 6) + e;
      word b = ((x * y) >> 6) - e;

      sendRGB(r, g, b);
    }
  }
  DISPLAY_DISCONNECT
}
