#include "SSD1306.h"
#include "command.h"

void SSD1306::init()
{
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
  for (int i = 0; i < 2000; i++)
    clear();
  send_command(SetDisplayOFF);
  send_command_list(init, sizeof(init));
  send_command(SetMultiplexRatio, LCD_MAX_Y);
  send_command(SetPrechargePeriod, 0xF1);

  if (LCD_MAX_Y == 31) send_command(SetPinsConfig, 0x02);
  else send_command(SetPinsConfig, 0x12);

  send_command(SetDisplayON);

}


void SSD1306::pixel(byte x, byte y)
{
  byte *pixel = &buffer[x + (y >> 3) * (LCD_MAX_X + 1)];
  byte bit = (1 << (y & 7));
  *pixel &= ~bit;
  *pixel |= _color ? bit : 0;
}

void SSD1306::rect_fill(uint8_t x, uint8_t y, uint8_t x1, uint8_t y1)
{
  for (byte xx = x; xx <= x1; xx++)
    for (byte yy = x; yy <= y1; yy++)
      pixel(xx, yy);
}

void SSD1306::symbol(byte *source, byte x, byte y, byte dx, byte dy)
{
  for (byte j = 0; j < dy; j++) {

    uint16_t offset = (uint16_t)source + (j >> 3) * dx;
    byte bit = 1 << (j & 7);

    for (byte i = 0; i < dx; i++) {

      byte data = pgm_read_byte(offset + i);
      if (data & bit) pixel(x + i, y + j);
    }
  }

}
