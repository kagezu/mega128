#ifndef SCREEN_H
#define SCREEN_H

#include "config.h"
#include "xpage/xpage.h"

typedef RGB(&rgb_bitmap)[1][WEIGHT];
#define BITMAP_SIZE WEIGHT * HEIGHT * sizeof(RGB)

class Screen :public Draw {
protected:
  const rgb_bitmap _bitmap;
  XPage *_page;
  byte _x = 0, _y = 0;
  RGB _color = RGB(0U);

public:
  Screen(XPage *page) :
    _bitmap((rgb_bitmap)*page->malloc(BITMAP_SIZE)),
    _page(page)
  {}

public:
  void point(byte x, byte y);
  void moveTo(byte x, byte y);
  void lineTo(byte x, byte y);
  void line(byte x1, byte y1, byte x2, byte y2);

};
#endif
