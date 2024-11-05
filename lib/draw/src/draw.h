#pragma once
#include <init.h>
#include "rgb18.h"

class Draw {
protected:
  const RGB(&_bitmap)[1][MAX_X + 1];
  const XPage *_page;
  const ST7735S *_lcd;

public:
  Draw(XPage *page, ST7735S *lcd) :
    _bitmap((RGB(&)[1][MAX_X + 1]) *page->create(128 * 160 * 3U)),
    _page(page),
    _lcd(lcd)
  {}



};
