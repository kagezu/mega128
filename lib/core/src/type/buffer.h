// #include "type/buffer.h"
#pragma once
#include <Arduino.h>

/*
#include <type/buffer.h>
#### Buffer<T, I>
+ read() <-- -- [ tail ] ... [ head ] <-- write()
+ shift() <-- - [ tail ] ... [ head ] <--  push()
+ unshift() --> [ tail ] ... [ head ] -->   pop()
+ tail()  =  buffer [ tail ]
+ head()  =  buffer [ head ]
*/
template <typename T, typename  I>
class Buffer {
protected:
  T *_buffer;
  I _head;    // Голова
  I _tail;    // Хвост
  I _size;    // Максимальный размер
  I _heap;    // Размер кучи

public:
  Buffer(I size);
  ~Buffer();
  void    clear();

  inline  I     length();
  inline  I     heap();
  inline  T     tail();
  inline  T     head();
  inline  T     shift(); // Alis read()

  void    write(T data);
  T       read();
  void    write(T *data, I length);
  void    read(T *data, I length);

  T       pop();
  I       unshift(T data);
  I       push(T data);
};

// Конструктор
template<typename T, typename I>
Buffer<T, I>::Buffer(I size)
{
  _buffer = (T *)malloc(size * sizeof(T));
  _size = size;
  clear();
}

// Деструктор
template<typename T, typename I>
Buffer<T, I>::~Buffer()
{
  free(_buffer);
}

// Очищает буфер
template<typename T, typename I>
void Buffer<T, I>::clear()
{
  _heap = _size;
  _head = _tail = 0;
}

// Текущий размер буфера
template<typename T, typename I>
inline I Buffer<T, I>::length()
{
  return _size - _heap;
}

// Свободный размер буфера
template<typename T, typename I>
inline I Buffer<T, I>::heap()
{
  return _heap;
}

// Возвращает элемент с головы, без удаления, даже если буфер пуст
template<typename T, typename I>
inline T Buffer<T, I>::head()
{
  return _buffer[_head ? _head - 1 : _size - 1];
}

// Возвращает элемент с хвоста, без удаления, даже если буфер пуст
template<typename T, typename I>
inline T Buffer<T, I>::tail()
{
  return _buffer[_tail];
}

// Возвращает элемент с хвоста либо пустой элемент, если буфер пуст
template<typename T, typename I>
inline T Buffer<T, I>::shift()
{
  return read();
}

// Записывает элемент в буфер, если буфер не полон
template<typename T, typename I>
void Buffer<T, I>::write(T data)
{
  if (!_heap) return;                       // Буфер полон
  _buffer[_head++] = data;
  _head = _head == _size ? 0 : _head;
  _heap--;
}

// Возвращает элемент из буфера либо пустой элемент, если буфер пуст
template<typename T, typename I>
T Buffer<T, I>::read()
{
  if (_heap == _size) return T();          // Буфер пуст
  I index = _tail++;
  _tail = _tail == _size ? 0 : _tail;
  _heap++;
  return _buffer[index];
}

// Записывает элементы в буфер до заполнения буфера, то что не влезло - обрезается
template<typename T, typename I>
void Buffer<T, I>::write(T *data, I length)
{
  T *target = _buffer + _head;
  I count = _size - _head;                  // Линейный размер пространства
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
template<typename T, typename I>
void Buffer<T, I>::read(T *data, I length)
{
  T *source = _buffer + _tail;
  I count = _size - _tail;                  // Линейный размер пространства
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

// Добавляет элемент с головы, возвращает его индекс
template<typename T, typename I>
I Buffer<T, I>::push(T data)
{
  if (!_heap) return -1;                 // Буфер полон
  I index = _head;
  _buffer[_head++] = data;
  _head = _head == _size ? I(0) : _head;
  _heap--;
  return index;
}

// Извлекает элемент с головы
template<typename T, typename I>
T Buffer<T, I>::pop()
{
  if (_heap == _size) return 0;          // Буфер пуст
  if (_head == 0) _head = _size - 1;
  _heap++;
  return _buffer[_head];
}

// Добавляет элемент в хвост, возвращает его индекс
template<typename T, typename I>
I Buffer<T, I>::unshift(T data)
{
  if (!_heap) return -1;                 // Буфер полон
  if (_tail == 0) _tail = _size;
  _buffer[--_tail] = data;
  _heap--;
  return _tail;
}
