#include <avr/pgmspace.h>
#include "SSD1306_I2C.h"
#include "command.h"

void SSD1306_I2C::command(byte command)
{
  twi.beginTransmission();
  twi.write(0x00);
  twi.write(command);
  twi.endTransmission();
}

void SSD1306_I2C::command(byte command, byte arg)
{
  twi.beginTransmission();
  twi.write(0x00);
  twi.write(command);
  twi.write(arg);
  twi.endTransmission();
}

void SSD1306_I2C::command(byte command, byte arg1, byte arg2)
{
  twi.beginTransmission();
  twi.write(0x00);
  twi.write(arg1);
  twi.write(arg2);
  twi.endTransmission();
}

void SSD1306_I2C::command_list(const byte *c, byte n)
{
  twi.beginTransmission();
  twi.write((byte)0x00);
  while (n--)
    twi.write(pgm_read_byte(c++));

  twi.endTransmission();
}

void SSD1306_I2C::begin(byte addr)
{

  twi.begin();
  twi.set_address(addr);

  delay(1000);

  command(SetDisplayOFF);
  static const byte PROGMEM init[] = {
    SetDisplayClock, 0x80,
    SetDisplayOffset, 0,
    SetDisplayStartLine,
    Set_0x8D, 0x14,
    SetMemoryAddressingMode, 0,
    SetVcomhDeselectLevel, 0x40,
    EntireDisplayON,
    SetNormalDisplay,
    DeactivateScroll
  };
  command_list(init, sizeof(init));

  command(SetMultiplexRatio, LCD_MAX_Y);
  command(SetPrechargePeriod, 0xF1);

  if ((LCD_MAX_X == 127) && (LCD_MAX_Y == 31))
    command(SetPinsConfig, 0x02);
  else if ((LCD_MAX_X == 127) && (LCD_MAX_Y == 63))
    command(SetPinsConfig, 0x12);

  command(SetDisplayON);

  clear();
}

void SSD1306_I2C::pixel(int16_t x, int16_t y, RGB color)
{
  byte *pixel = &buffer[x + (y >> 3) * (LCD_MAX_X + 1)];
  byte bit = (1 << (y & 7));
  *pixel &= ~bit;
  *pixel |= color ? bit : 0;
}

void SSD1306_I2C::clear(void)
{
  memset(buffer, 0, (LCD_MAX_X + 1) * ((LCD_MAX_Y >> 8) + 1));
}

void SSD1306_I2C::update(void)
{
  static const byte PROGMEM dlist1[] = {
    SetPageAddress,
    0,
    0xFF,
    SetColumnAddress,
    0 };
  command_list(dlist1, sizeof(dlist1));
  command(LCD_MAX_X);

  uint16_t count = (LCD_MAX_X + 1) * ((LCD_MAX_Y >> 3) + 1);

  twi.beginTransmission();
  twi.write((byte)0x40);
  twi.write(buffer, count);
  twi.endTransmission();
}
