#ifndef DRAW_H
#define DRAW_H

#include "display/config.h"
#include "rgb/rgb.h"

class  Draw {
protected:
  byte _fat = 2;

public:
  void fat(byte f) { _fat = f; }

  // Реализуется в данном классе
public:
  // Линии и фигуры
  void w_line(byte, byte, byte);
  void h_line(byte, byte, byte);
  void w_line_fat(byte, byte, byte);
  void h_line_fat(byte, byte, byte);

  void line(byte, byte, byte, byte);
  void round_rect(byte, byte, byte, byte, byte);
  void rect_line(byte, byte, byte, byte);
  void circle(byte, byte, byte);

  // Утолщённые линии и фигуры
  void line_fat(byte, byte, byte, byte);
  void rect_fat(byte, byte, byte, byte);
  void circle_fat(byte, byte, byte);

  // Полные фигуры
  void circle_fill(byte, byte, byte);
  void round_rect_fill(byte, byte, byte, byte, byte);

  // Реализуется в дочернем классе
public:
  virtual void pixel(byte, byte) = 0;
  virtual void rect_fill(byte, byte, byte, byte) = 0;
};
#endif
