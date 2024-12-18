#include <Arduino.h>

/*
   read() <-- [ tail ] ... [ head ] <-- write()
  shift() <-- [ tail ] ... [ head ] <--  push()
unshift() --> [ tail ] ... [ head ] -->   pop()

    at(x)  = & array [ x ]
   tail()  = & array [ tail ]
   head()  = & array [ head ]
   circ()  = & array [ tail ] --> [ head ]
 uncirc()  = & array [ tail ] <-- [ head ]
 erase(X)    [ X ] <-- ... <-- [ head ] X
.              !   -->    -->    -->    i
*/

template <typename T, typename  S>
class Array {
protected:
  T *_array;
  S *_buffer;
  S _head;                                  // Голова
  S _tail;                                  // Хвост
  S _size;                                  // Максимальный размер
  S _heap;                                  // Размер кучи

public:
  Array(S size);
  ~Array();
  void       clear();

  inline  S  length();
  inline  S  heap();
  inline  T *tail();
  inline  T *head();
  inline  T *at(S index);

  void write(T *data, S length);
  void write(T data);
  void read(T *data, S length);
  T    read();
  S    push(T data);
  T    pop();
  S    unshift(T data);
  T    shift(); // Alis read()

  void erase(S index);
  T *circ();
  T *uncirc();

protected:
  S    seek(S index);
};

// Конструктор
template<typename T, typename S>
Array<T, S>::Array(S size)
{
  _buffer = (S *)malloc(size * sizeof(S));
  _array = (T *)malloc(size * sizeof(T));
  _size = size;
  clear();
}

// Деструктор
template<typename T, typename S>
Array<T, S>::~Array()
{
  free(_array);
  free(_buffer);
}

// Очищает массив
template<typename T, typename S>
void Array<T, S>::clear()
{
  _heap = _size;
  _head = _tail = S(0);
  for (S i = 0; i < _size; i++)
    _buffer[i] = i;
}

// Текущий размер буфера
template<typename T, typename S>
inline S Array<T, S>::length()
{
  return _size - _heap;
}

// Свободный размер буфера
template<typename T, typename S>
inline S Array<T, S>::heap()
{
  return _heap;
}

// Возвращает элемент с головы, без удаления, даже если буфер пуст
template<typename T, typename S>
inline T *Array<T, S>::head()
{
  return &_array[_buffer[_head ? _head - 1 : _size - 1]];
}

// Возвращает элемент с хвоста, без удаления, даже если буфер пуст
template<typename T, typename S>
inline T *Array<T, S>::tail()
{
  return &_array[_buffer[_tail]];
}

// Возвращает элемент с хвоста либо пустой элемент, если буфер пуст
template<typename T, typename S>
inline T Array<T, S>::shift()
{
  return read();
}

// Записывает элемент в буфер, если буфер не полон
template<typename T, typename S>
void Array<T, S>::write(T data)
{
  if (!_heap) return;                       // Буфер полон
  _array[_buffer[_head++]] = data;
  _head = _head == _size ? S(0) : _head;
  _heap--;
}

// Возвращает элемент из буфера либо пустой элемент, если буфер пуст
template<typename T, typename S>
T Array<T, S>::read()
{
  if (_heap == _size) return _array[0];     // Буфер пуст
  S index = _tail++;
  _tail = _tail == _size ? S(0) : _tail;
  _heap++;
  return _array[_buffer[index]];
}

// Записывает элементы в буфер до заполнения буфера, то что не влезло - обрезается
template<typename T, typename S>
void Array<T, S>::write(T *data, S length)
{
  T *target = _buffer + _head;
  S count = _size - _head;                  // Линейный размер пространства
  length = length > _heap ? _heap : length; // Размер перемещаемых данных ограничен кучей
  _heap -= length;                          // Уменьшаем кучу
  _head += length;                          // Двигаем голову
  if (length >= count) {                    // Если будет достигнут конец буферного пространства
    length -= count;                        // Остаток на запись
    _head = length;                         // Новое положение головы
    while (count--) _array[*target++] = *data++;
    target = _buffer;                       // продолжим запись с начала буфера
  }
  while (length--) _array[*target++] = *data++;
}

// Возвращает элементы из буфера до опустошения
template<typename T, typename S>
void Array<T, S>::read(T *data, S length)
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
    while (count--) *data++ = _array[*source++];
    source = _buffer;                       // Продолжим читать с начала буфера
  }
  while (length--) *data++ = _array[*source++];
}

// Добавляет элемент с головы, возвращает его индекс
template<typename T, typename S>
S Array<T, S>::push(T data)
{
  if (!_heap) return _size;                 // Буфер полон
  S index = _head;
  _array[_buffer[_head++]] = data;
  _head = _head == _size ? S(0) : _head;
  _heap--;
  return index;
}

// Извлекает элемент с головы
template<typename T, typename S>
T Array<T, S>::pop()
{
  if (_heap == _size) return _array[0];     // Буфер пуст
  _head = _head == S(0) ? _size - 1 : _head - 1;
  _heap++;
  return _array[_buffer[_head]];
}

// Добавляет элемент в хвост, возвращает его индекс
template<typename T, typename S>
S Array<T, S>::unshift(T data)
{
  if (!_heap) return _size;                 // Буфер полон
  if (_tail == 0) _tail = _size;
  _array[_buffer[--_tail]] = data;
  _heap--;
  return _tail;
}

// Возвращает указатель на элемент по индексу
template<typename T, typename S>
inline T *Array<T, S>::at(S index)
{
  return &_array[index];
}

// Находит индекс в буфере
template<typename T, typename S>
S Array<T, S>::seek(S index)
{
  S i = _tail;
  while (i != _head) {
    if (_buffer[i] == index) break;
    if (++i == _size) i = S(0);
  }
  if (i == _head) i = -1;                   // Индекс не найден
  return i;
}

template<typename T, typename S>
void Array<T, S>::erase(S index)
{
  S i = _tail, j;
  while (i != _head) {
    if (_buffer[i] == index) break;
    if (++i == _size) i = S(0);
  }
  if (i == _head)  return;                  // Индекс не найден
  while (i != _head) {
    j = i;
    if (++i == _size) i = S(0);
    _buffer[j] = _buffer[i];                // Сдвиг индексации
  }
  _buffer[j] = index;
}

template<typename T, typename S>
T *Array<T, S>::circ()
{
  S last = _buffer[_heap];
  _buffer[_heap] = _buffer[_head];
  _buffer[_head] = last;

  _head = ++_head == _size ? S(0) : _head;
  _heap = ++_heap == _size ? S(0) : _heap;

  return &_array[last];
}

template<typename T, typename S>
T *Array<T, S>::uncirc()
{
  S top = _buffer[_head];
  _buffer[_head] = _buffer[_heap];
  _buffer[_heap] = top;

  _head = _head == S(0) ? _size - 1 : _head - 1;
  _heap = _heap == S(0) ? _size - 1 : _heap - 1;

  return &_array[top];
}
