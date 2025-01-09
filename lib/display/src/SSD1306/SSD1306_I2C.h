#pragma once

#include <twi.h>
#include "rgb/rgb.h"
#include "display/config.h"

class SSD1306_I2C {
public:

  void begin(byte);
  void update();
  void clear();
  void pixel(int16_t x, int16_t y, RGB color);

private:
  void command(byte);
  void command(byte, byte);
  void command(byte, byte, byte);
  void command_list(const byte *c, byte n);

  TWI_Master twi;
  byte buffer[(LCD_MAX_X + 1) * ((LCD_MAX_Y >> 3) + 1)];
};
