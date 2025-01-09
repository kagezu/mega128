#include "SSD1306.h"
#include "command.h"

void SSD1306::init()
{
  static const byte PROGMEM init[] = {
    SetDisplayClock, 0x80,
    SetDisplayOffset, 0,
    SetDisplayStartLine,
    Set_0x8D, 0x14,//CHARGE_DCDC 0x10 off  0x14 on
    SetMemoryAddressingMode, 0,
    SetVcomhDeselectLevel, 0x40,//	Устанавливаем питание светодиодов VcomH в значение выше чем по умолчанию (0x30), что увеличит яркость дисплея (допустимые значения: 0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70).
    EntireDisplayON,
    SetNormalDisplay,
    DeactivateScroll
  };
  for (int i = 0; i < 2000; i++)
    clear();
  TCCR0B = 1;
  sei();
  send_command(SetDisplayOFF);
  send_command_list(init, sizeof(init));
  send_command(SetMultiplexRatio, 0x1f);
  send_command(SetPrechargePeriod, 0xF1);

#ifndef LCD_BREAK
  send_command(SetPinsConfig, 0x02);
#else 
  send_command(SetPinsConfig, 0x22);
#endif

  send_command(SetDisplayON);

}


void SSD1306::pixel(byte x, byte y)
{
  if (x > MAX_X || y > MAX_Y) return;
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
