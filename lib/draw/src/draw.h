#ifndef DRAW_H
#define DRAW_H

#include "init.h"

typedef RGB(&rgb_bitmap)[1][MAX_X];
#define BITMAP_SIZE MAX_Y * MAX_X * sizeof(RGB)

class Draw {
protected:
  const rgb_bitmap _bitmap;
  const XPage *_page;
  const ST7735S *_lcd;
  byte _x = 0, _y = 0;
  RGB _color = 0;

public:
  Draw(XPage *page, ST7735S *lcd) :
    _bitmap((rgb_bitmap)*page->create(BITMAP_SIZE)),
    _page(page),
    _lcd(lcd)
  {}

public:
  void point(byte x, byte y);
  void moveTo(byte x, byte y);
  void lineTo(byte x, byte y);
  void line(byte x1, byte y1, byte x2, byte y2);

};
#endif
