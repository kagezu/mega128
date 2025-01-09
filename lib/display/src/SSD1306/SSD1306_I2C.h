#pragma once

#include <twi.h>
#include "rgb/rgb.h"
#include "display/config.h"

class SSD1306_I2C {
public:
  SSD1306_I2C();
  void update();

protected:
  byte buffer[(LCD_MAX_X + 1) * ((LCD_MAX_Y >> 3) + 1)];

  void send_command(byte);
  void send_command(byte, byte);
  void send_command(byte, byte, byte);
  void send_command_list(const byte *, byte);

private:
  TWI_Master twi;
};
