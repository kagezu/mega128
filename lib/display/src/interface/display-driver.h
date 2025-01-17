// #include <interface/display-driver.h>
class RGB;

template<typename I>
class DisplayDriver {
public:
  virtual void color(RGB c) = 0;
  virtual void background(RGB) = 0;
  virtual void clear() = 0;
  virtual void clear(RGB color) = 0;
  virtual void symbol(byte *, I, I, byte, byte) = 0;
  virtual void pixel(I, I) = 0;
  virtual void rect_fill(I, I, I, I) = 0;
};
