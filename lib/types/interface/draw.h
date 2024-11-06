#include "rgb/rgb.h"

class  Draw {
protected:
  uint8_t _pointX = 0;
  uint8_t _pointY = 0;
  uint8_t _fat = 1;
  RGB _color = 0U;
  RGB _background = 0U;
  uint8_t _transparent = 0;

public:
  void point(uint8_t x, uint8_t y) { _pointX = x; _pointY = y; }
  void fat(uint8_t f) { _fat = f; }
  void color(RGB c) { _color = c; }
  void background(RGB b) { _background = b; }
  void transparent(uint8_t t) { _transparent = t; }

public:
  void pixel(uint8_t x, uint8_t y) { pixel(x, y, _color); }
  void lineTo(uint8_t x, uint8_t y) { line(_pointX, _pointY, x, y); _pointX = x; _pointY = y; }
  void lineToFat(uint8_t x, uint8_t y) { lineFat(_pointX, _pointY, x, y); _pointX = x; _pointY = y; }

public:
  virtual void pixel(uint8_t x, uint8_t y, RGB color);
  virtual void line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
  virtual void circle(uint8_t x, uint8_t y, uint8_t  radius;
  virtual void rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
  virtual void roundRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t radius);

  virtual void lineFat(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
  virtual void circleFat(uint8_t x, uint8_t y, uint8_t  radius);
  virtual void rectFat(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
  virtual void roundRectFat(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t  radius);

  virtual void circleFill(uint8_t x, uint8_t y, uint8_t  radius);
  virtual void rectFill(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
  virtual void roundRectFill(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t  radius);


};
