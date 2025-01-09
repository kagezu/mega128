#include <avr/pgmspace.h>
#include "SSD1306_I2C.h"
#include "command.h"

SSD1306_I2C::SSD1306_I2C()
{
  twi.init();
  twi.set_address(LCD_I2C_ADDR);
}

void SSD1306_I2C::send_command(byte command)
{
  twi.begin();
  twi.write(0);
  twi.write(command);
  twi.end();
}

void SSD1306_I2C::send_command(byte command, byte arg)
{
  twi.begin();
  twi.write(0);
  twi.write(command);
  twi.write(arg);
  twi.end();
}

void SSD1306_I2C::send_command(byte command, byte arg1, byte arg2)
{
  twi.begin();
  twi.write(0);
  twi.write(command);
  twi.write(arg1);
  twi.write(arg2);
  twi.end();
}

void SSD1306_I2C::send_command_list(const byte *command, byte count)
{
  twi.begin();
  twi.write(0);
  while (count--) twi.write(pgm_read_byte(command++));
  twi.end();
}

void SSD1306_I2C::update(void)
{
  uint16_t count = (LCD_MAX_X + 1) * ((LCD_MAX_Y >> 3) + 1);

  send_command(SetPageAddress, 0, 0xFF);
  send_command(SetColumnAddress, 0, LCD_MAX_X);

  twi.begin();
  twi.write((byte)0x40);
  twi.write(buffer, count);
  twi.end();
}
