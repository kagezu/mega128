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

  command(SetDisplayOFF);
  command_list(init, sizeof(init));
  command(SetMultiplexRatio, LCD_MAX_Y);
  command(SetPrechargePeriod, 0xF1);

  if (LCD_MAX_Y == 31) command(SetPinsConfig, 0x02);
  else command(SetPinsConfig, 0x12);

  command(SetDisplayON);

  clear();
}
