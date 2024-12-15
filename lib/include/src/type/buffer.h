#include <Arduino.h>

/*
   read() <-- [tail] ... [head] <-- write()
  shift() <-- [tail] ... [head] <--  push()
unshift() --> [tail] ... [head] -->   pop()

    at(i)  =  buffer[i]
 peekIn()  =  at( head )
peekOut()  =  at( tail )
*/

template <typename T, typename  S>
class Buffer {
protected:
  T *_buffer;
  S _head;                                    // Голова
  S _tail;                                    // Хвост
  S _size;                                    // Максимальный размер
  S _heap;                                    // Размер кучи

public:
  Buffer(S size);
  ~Buffer();
  void    clear();

  inline  S     length();
  inline  S     heap();
  inline  T     peekIn();
  inline  T     peekOut();
  inline  T     shift(); // Alis read()
  inline  T     at(S index);

  void    write(T data);
  T       read();
  void    write(T *data, S length);
  void    read(T *data, S length);

  boolean isIndex(S index);
  T       pop();
  S       unshift(T data);
  S       push(T data);
};

// Конструктор
template<typename T, typename S>
Buffer<T, S>::Buffer(S size)
{
  _buffer = (T *)malloc(size * sizeof(T));
  _size = size;
  clear();
}

// Деструктор
template<typename T, typename S>
Buffer<T, S>::~Buffer()
{
  free(_buffer);
}

// Очищает буфер
template<typename T, typename S>
void Buffer<T, S>::clear()
{
  _heap = _size;
  _head = _tail = S(0);
}

// Текущий размер буфера
template<typename T, typename S>
inline S Buffer<T, S>::length()
{
  return _size - _heap;
}

// Свободный размер буфера
template<typename T, typename S>
inline S Buffer<T, S>::heap()
{
  return _heap;
}

// Возвращает элемент с головы, без удаления, даже если буфер пуст
template<typename T, typename S>
inline T Buffer<T, S>::peekIn()
{
  return _buffer[_head ? _head - 1 : _size - 1];
}

// Возвращает элемент с хвоста, без удаления, даже если буфер пуст
template<typename T, typename S>
inline T Buffer<T, S>::peekOut()
{
  return _buffer[_tail];
}

// Возвращает элемент с хвоста либо пустой элемент, если буфер пуст
template<typename T, typename S>
inline T Buffer<T, S>::shift()
{
  return read();
}

// Записывает элемент в буфер, если буфер не полон
template<typename T, typename S>
void Buffer<T, S>::write(T data)
{
  if (!_heap) return;                       // Буфер полон
  _buffer[_head++] = data;
  _head = _head == _size ? S(0) : _head;
  _heap--;
}

// Возвращает элемент из буфера либо пустой элемент, если буфер пуст
template<typename T, typename S>
T Buffer<T, S>::read()
{
  if (_heap == _size) return T(0);          // Буфер пуст
  S index = _tail++;
  _tail = _tail == _size ? S(0) : _tail;
  _heap++;
  return _buffer[index];
}

// Записывает элементы в буфер до заполнения буфера, то что не влезло - обрезается
template<typename T, typename S>
void Buffer<T, S>::write(T *data, S length)
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
template<typename T, typename S>
void Buffer<T, S>::read(T *data, S length)
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

// Проверяет существование элемента по индексу
template<typename T, typename S>
boolean Buffer<T, S>::isIndex(S index)
{
  boolean result = false;
  if (index < _size && length())            // Если индекс не выходит за пределы буфера и буфер не пуст
    if (_head > _tail) {                    // Находиться ли индекс между указателями в зависимости от их положения
      if (index >= _tail && index < _head) result = true;
    }
    else if (index >= _tail || index < _head) result = true;
  return result;
}

// Добавляет элемент с головы, возвращает его индекс
template<typename T, typename S>
S Buffer<T, S>::push(T data)
{
  if (!_heap) return S(-1);                 // Буфер полон
  S index = _head;
  _buffer[_head++] = data;
  _head = _head == _size ? S(0) : _head;
  _heap--;
  return index;
}

// Извлекает элемент с головы
template<typename T, typename S>
T Buffer<T, S>::pop()
{
  if (_heap == _size) return T(0);          // Буфер пуст
  if (_head == 0) _head = _size - 1;
  _heap++;
  return _buffer[_head];
}

// Добавляет элемент в хвост, возвращает его индекс
template<typename T, typename S>
S Buffer<T, S>::unshift(T data)
{
  if (!_heap) return S(-1);                 // Буфер полон
  if (_tail == 0) _tail = _size;
  _buffer[--_tail] = data;
  _heap--;
  return _tail;
}

// Возвращает элемент по индексу
template<typename T, typename S>
inline T Buffer<T, S>::at(S index)
{
  return _buffer[index];
}

