#pragma once

#include "display/config.h"
#include "comands.h"
#include "rgb/rgb.h"

class ST7735_Soft {
public:
  void send_command(byte data);
  void set_addr(byte x0, byte y0, byte x1, byte y1);
  void send_zero();
  void send_byte(byte data);
  void send_rgb(byte r, byte g, byte b);
  void send_rgb(uint16_t data); // формат 0x0rgb / RGB_16
  void send_rgb(uint32_t color);
  void send_rgb(RGB color);
  void rect(byte x0, byte y0, byte x1, byte y1, RGB color);
};
