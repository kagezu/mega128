#include <Arduino.h>

/*
   peek()  =  [tail]
   read() <-- [tail] ... [head] <-- write()
  shift() <-- [tail] ... [head] <--  push()
unshift() --> [tail] ... [head] -->   pop()
*/

template <typename T, typename  S>
class Buffer {
protected:
  T *_buffer;
  S _head;    // Голова
  S _tail;    // Хвост
  S _size;    // Максимальный размер
  S _heap;    // Размер кучи

public:
  Buffer(S size)
  {
    _buffer = (T *)malloc(size * sizeof(T));
    _size = size;
    clear();
  }
  ~Buffer() { free(_buffer); }

  // Очищает буфер
  inline void clear()
  {
    _heap = _size;
    _head = _tail = S(0);
  }

  // Текущий размер буфера
  inline S length() { return _size - _heap; }

  // Свободный размер буфера
  inline S heap() { return _heap; }

  // Возвращает последний элемент, даже если буфер пуст
  inline T peek() { return _buffer[_tail]; }

  // Записывает элемент в буфер, если буфер не полон
  void write(T data)
  {
    if (!_heap) return; // Буфер полон
    _buffer[_head++] = data;
    if (_head == _size) _head = S(0);
    _heap--;
  }

  // Возвращает элемент из буфера либо пустой элемент, если буфер пуст
  T read()
  {
    if (_heap == _size) return T(0);    // Буфер пуст
    T data = _buffer[_tail++];
    if (_tail == _size) _tail = S(0);
    _heap++;
    return data;
  }

  // Записывает элементы в буфер до заполнения
  void write(T *data, S length)
  {
    T *target = _buffer + _head;
    S count = _size - _head;                  // Линейный размер пространства
    length = length > _heap ? _heap : length; // Размер перемещаемых данных ограничен кучей
    _heap -= length;                          // Уменьшаем кучу
    _head += length;                          // Двигаем голову
    if (length >= count) {                    // Если будет достигнут конец буферного пространства
      length -= count;                        // Остаток на запись
      _head = length;                         // Новое положение головы
      while (count--)*target++ = *data++;
      target = _buffer;                       // продолжим запись с начала буфера
    }
    while (length--)*target++ = *data++;
  }

  // Возвращает элементы из буфера до опустошения
  void read(T *data, S length)
  {
    T *source = _buffer + _tail;
    S count = _size - _tail;                  // Линейный размер пространства
    length = length > _size - _heap
      ? _size - _heap : length;               // Размер перемещаемых данных ограничен их количеством
    _heap += length;                          // высвобождаем кучу
    _tail += length;                          // Двигаем хвост
    if (length >= count) {                    // Если будет достигнут конец буферного пространства
      length -= count;                        // Остаток на считывание
      _tail = length;                         // Новое положение хвоста
      while (count--) *data++ = *source++;
      source = _buffer;                       // Продолжим читать с начала буфера
    }
    while (length--) *data++ = *source++;
  }

  // Извлекает последний элемент
  T pop()
  {
    if (_heap == _size) return T(0);    // Буфер пуст
    if (_head == 0) _head = _size;
    T data = _buffer[--_head];
    _heap++;
    return data;
  }

  // Добавляет первый элемент
  void unshift(T data)
  {
    if (!_heap) return;                 // Буфер полон
    if (_tail == 0) _tail = _size;
    _buffer[--_tail] = data;
    _heap--;
  }

  // Alis
  inline void push(T data) { write(data); }
  inline T shift() { return read(); }
};
