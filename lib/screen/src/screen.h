#ifndef SCREEN_H
#define SCREEN_H

#include "config.h"
#include "xpage/xpage.h"

// Обход ограничения 0x8000 байт на массив
typedef RGB(&rgb_bitmap)[1][WEIGHT];
#define BITMAP_SIZE WEIGHT * HEIGHT * sizeof(RGB)

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

  // Реализация интерфейса Draw
public:
  void pixel(uint8_t x, uint8_t y, RGB color);
  void rectFill(uint8_t x, uint8_t y, uint8_t x1, uint8_t y1);

public:
  void clear(RGB color);
  void copyBitmap(uint8_t x, uint8_t y, uint8_t width, uint8_t height, RGB *source);
};

#endif
