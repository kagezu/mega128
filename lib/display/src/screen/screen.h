#ifndef SCREEN_H
#define SCREEN_H

#include "display.config.h"
#include "draw/draw.h"
#include "../../../xmem/src/xpage/xpage.h"

// Обход ограничения 0x8000 байт на массив
typedef RGB(&rgb_bitmap)[1][MAX_X + 1];
#define BITMAP_SIZE (MAX_X + 1) * (MAX_Y + 1) * sizeof(RGB)

class Screen :public Draw {
protected:
  rgb_bitmap _bitmap;
  XPage *_page;

public:
  Screen(XPage *page) :
    _bitmap((rgb_bitmap)*page->malloc(BITMAP_SIZE)),
    _page(page)
  {}

  // Специфические для данного класса
public:
  inline RGB *getSource() { return  _bitmap[0]; }
  void saveBitmap(uint8_t x, uint8_t y, uint8_t width, uint8_t height, RGB *target);
  void copyBitmap(uint8_t x, uint8_t y, uint8_t width, uint8_t height, RGB *source);

  // Реализация интерфейса Draw
public:
  void pixel(uint8_t x, uint8_t y, RGB color);
  void rectFill(uint8_t x, uint8_t y, uint8_t x1, uint8_t y1);
  void clear(RGB color);
};

#endif
