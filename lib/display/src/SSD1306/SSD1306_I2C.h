#pragma once
#include "display/default.h"
#ifdef _SSD1306_

#include <twi.h>
#include "rgb/rgb.h"

class SSD1306_I2C {
public:
  SSD1306_I2C();
  void update();

protected:
  uint8_t buffer[(LCD_MAX_X + 1) * ((LCD_MAX_Y >> 3) + 1)];

  void send_command(uint8_t);
  void send_command(uint8_t, uint8_t);
  void send_command(uint8_t, uint8_t, uint8_t);
  void send_command_list(const uint8_t *, uint8_t);

private:
  TWI_Master twi;
};

#endif
