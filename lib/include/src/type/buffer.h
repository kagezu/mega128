#include <Arduino.h>

template <typename T, typename  S>
class Buffer {
protected:
  T *_buffer;
  S _head;
  S _tail;
  S _size;
  S _amount;// Заполненность буфера

public:
  Buffer(S size) : _buffer((T *)malloc(size * sizeof(T))), _size(size) { clear(); }
  ~Buffer() { free(_buffer); }

  // Очищает буфер
  inline void clear() { _amount = _head = _tail = S(0); }

  // Текущий размер буфера
  inline S amount() { return _amount; }

  // Свободный размер буфера
  inline S heap() { return _size - _amount; }

  // Возвращает последний элемент, даже если буфер пуст
  inline T peek() { return _buffer[_tail]; }

  // Записывает элемент в буфер, если буфер не полон
  void write(T data)
  {
    if (_amount == _size) return; // Буфер полон
    *_buffer[_head++] = data;
    if (_head == _size) _head = S(0);
    _amount++;
  }

  // Возвращает элемент из буфера либо пустой элемент, если буфер пуст
  T read()
  {
    if (!_amount) return T(0);    // Буфер пуст
    T data = _buffer[_tail++];
    if (_tail == _size) _tail = S(0);
    return data;
  }

  // Возвращает количество успешно записанных элементов
  S write(T *data, S length)
  {
    T *target = _buffer + _head;
    S count = heap();
    length = count >= length ? length : count;

    if (length <= _size - _head)
      while (length--)*target++ = *data++;
  }

  // Возвращает количество успешно считанных элементов
  S read(T *data, S length)
  {
    if (!_amount) return T(0);    // Буфер пуст
    T data = _buffer[_tail++];
    if (_tail == _size) _tail = S(0);
    return data;
  }




};
