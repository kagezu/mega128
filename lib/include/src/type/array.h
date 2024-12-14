#include "buffer.h"

template <typename T, typename  S>
class Array {
protected:

public:
  Array(S size) :Buffer<T, S>::Buffer(size) {}
  ~Array() {}

  // извлекаем последний элемент
  T pop()
  {
    if (_heap == _size) return T(0);    // Буфер пуст
    if (_head == 0) _head = _size;
    T data = _buffer[--_head];
    _heap++;
    return data;
  }
};
