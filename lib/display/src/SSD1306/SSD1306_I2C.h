#pragma once

#include <twi.h>
#include "rgb/rgb.h"
#include "display/config.h"

class SSD1306_I2C {
public:
  SSD1306_I2C();

  void update();
  void clear();
  void pixel(int16_t x, int16_t y, RGB color);

protected:
  byte buffer[(LCD_MAX_X + 1) * ((LCD_MAX_Y >> 3) + 1)];

  void command(byte);
  void command(byte, byte);
  void command(byte, byte, byte);
  void command_list(const byte *, byte);

  // private:
  TWI_Master twi;
};
