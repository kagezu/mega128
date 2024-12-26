#include <Arduino.h>
#pragma once

/*
#### Array<T, I> T элемент, I индекс
+ read () <--- [ tail ] .. [ head ] <-- write()
+ shift() <--- [ tail ] .. [ head ] <--  push()
+ unshift() -> [ tail ] .. [ head ] -->   pop()
+ back() <= shift () --> push()
+ circ() = unshift() <-- pop()
+ tail() / head() = & [ tai l] / [ head ]
+ erase(X) / drop(x) [ X ] <--  <-- [ head ]
+ find() / filter() / forEach()
*/
template <typename T, typename  I>
class Array {
protected:
  T *_array;
  I *_index;
  const I _size;       // Максимальный размер
  volatile I _head;    // Голова
  volatile I _tail;    // Хвост
  volatile I _heap;    // Размер кучи

public:
  /*
    Array(I size);
    ~Array();
    void       clear();
    inline  I  length();
    inline  I  heap();

    // Чтение и запись элементов

    inline  T *tail();
    inline  T *head();
    inline  T *at(I index);

    void write(T *data, I length);
    void write(T data);
    void read(T *data, I length);
    T    read();
    I    push(T data);
    T    pop();
    I    unshift(T data);
    T    shift(); // Alis read()
    T *circ();
    T *unCirc();

    // Действия над элементами

    void erase(I index);
    void filter(bool callback(T *)); // Удалить несоответствующие элементы
    void forEach(callback(T *)); // Перебор элементов
    T *find(bool callback(T *)); // Поиск элемента
    I findindex(bool callback(T *)); // Поиск элемента
    I indexOf(T *); // Поиск элемента

  // Не реализовано

  virtual void flat(); // Убрать дыры, обновить индексацию
  virtual bool some(bool callback(T *)); // Проверка на наличие
  virtual bool every(bool callback(T *)); // Проверка на отсутствие
  virtual bool fill(T, I, I); // Заполнение
  virtual bool sort(bool compare(T *, T *)); // Сортировка
  virtual bool splice(I start, I deleteCount = 1, T *items = nullptr, I count = 0); // Удаление/замена элементов

  */

public:
  Array(I size) :_size(size)
  {
    _index = (I *)malloc(size * sizeof(I));
    _array = (T *)malloc(size * sizeof(T));
    clear();
  }
  ~Array() { free(_array); free(_index); }

  // Очищает массив
  void clear()
  {
    _heap = _size;
    _head = _tail = 0;
    for (I i = 0; i < _size; i++) _index[i] = i;
  }

  // Текущий размер массива
  inline I length() { return _size - _heap; }

  // Свободный размер массива
  inline I heap() { return _heap; }

  // Возвращает элемент с головы, без удаления, даже если буфер пуст
  inline T *head() { return &_array[_index[_head ? _head - 1 : _size - 1]]; }

  // Возвращает элемент с хвоста, без удаления, даже если буфер пуст
  inline T *tail() { return &_array[_index[_tail]]; }

  // Записывает элемент в буфер, если буфер не полон
  void write(T data)
  {
    if (!_heap) return; // Буфер полон
    _array[_index[_head]] = data;
    _incHead();
    _heap--;
  }

  // Возвращает элемент из буфера либо пустой элемент, если буфер пуст
  T read()
  {
    if (_heap == _size) return T(); // Буфер пуст
    I index = _tail;
    _incTail();
    _heap++;
    return _array[_index[index]];
  }

  // Записывает элементы в буфер до заполнения буфера, то что не влезло - обрезается
  // не реализован
  void write(T *data, I length)
  {
    T *target = _index + _head;
    I count = _size - _head;                  // Линейный размер пространства
    length = length > _heap ? _heap : length; // Размер перемещаемых данных ограничен кучей
    _heap -= length;                          // Уменьшаем кучу
    _head += length;                          // Двигаем голову
    if (length >= count) {                    // Если будет достигнут конец буферного пространства
      length -= count;                        // Остаток на запись
      _head = length;                         // Новое положение головы
      while (count--) _array[*target++] = *data++;
      target = _index;                        // продолжим запись с начала буфера
    }
    while (length--) _array[*target++] = *data++;
  }

  // Возвращает элементы из буфера до опустошения
   // не реализован
  void read(T *data, I length)
  {
    T *source = _index + _tail;
    I count = _size - _tail;                  // Линейный размер пространства
    length = length > _size - _heap
      ? _size - _heap : length;               // Размер перемещаемых данных ограничен их количеством
    _heap += length;                          // высвобождаем кучу
    _tail += length;                          // Двигаем хвост
    if (length >= count) {                    // Если будет достигнут конец буферного пространства
      length -= count;                        // Остаток на считывание
      _tail = length;                         // Новое положение хвоста
      while (count--) *data++ = _array[*source++];
      source = _index;                        // Продолжим читать с начала буфера
    }
    while (length--) *data++ = _array[*source++];
  }

  // Добавляет элемент с головы, возвращает его адрес
  T *push(T data)
  {
    if (!_heap) return nullptr; // Буфер полон
    T *ptr = &_array[_index[_head]];
    *ptr = data;
    _incHead();
    _heap--;
    return ptr;
  }

  // Добавляет элемент с головы, возвращает его адрес
  T *push()
  {
    if (!_heap) return nullptr; // Буфер полон
    T *ptr = &_array[_index[_head]];
    _incHead();
    _heap--;
    return ptr;
  }

  // Добавляет элемент в хвост, возвращает его адрес
  T *unshift(T data)
  {
    if (!_heap) return nullptr; // Буфер полон
    _decTail();
    _heap--;
    T *ptr = &_array[_index[_tail]];
    *ptr = data;
    return ptr;
  }

  // Добавляет элемент в хвост, возвращает его адрес
  T *unshift()
  {
    if (!_heap) return nullptr; // Буфер полон
    _decTail();
    _heap--;
    return &_array[_index[_tail]];
  }

  // Извлекает элемент с головы
  T *pop()
  {
    if (_heap == _size) return nullptr; // Буфер пуст
    _decHead();
    _heap++;
    return &_array[_index[_head]];
  }

  // Возвращает элемент с хвоста либо пустой элемент, если буфер пуст
  T *shift()
  {
    if (_heap == _size) return nullptr; // Буфер пуст
    I index = _tail;
    _incTail();
    _heap++;
    return &_array[_index[index]];
  }

  // Переносит элемент с хвоста в голову
  T *circ()
  {
    I last = _index[_tail];
    _index[_tail] = _index[_head];
    _index[_head] = last;
    _incHead();
    _incTail();
    return &_array[last];
  }

  // Переносит элемент с головы в хвост
  T *back()
  {
    _decHead();
    _decTail();
    I top = _index[_head];
    _index[_head] = _index[_tail];
    _index[_tail] = top;
    return &_array[top];
  }

  // Выкидывает элемент из массива
  T *drop(T *ptr)
  {
    if (ptr == nullptr) return nullptr;
    I index = ptr - _array;
    _seek(index);
    if (_count == _head)  return nullptr; // Индекс не найден
    _erase();
    return ptr;
  }

  // Выкидывает элемент из массива
  void erase(T *ptr)
  {
    if (ptr == nullptr) return;
    I index = ptr - _array;
    _seek(index);
    if (_count == _head)  return; // Индекс не найден
    _erase();
  }

  // Поиск элемента
  T *find(bool callback(T *))
  {
    I i = _size;
    _count = _tail;
    while (i--) {
      T *element = &_array[_index[_count]];
      if (callback(element)) return element;
      _incCount();
    }
    return nullptr;
  }

  // Перебор элементов
  void forEach(void callback(T *))
  {
    I i = _size;
    _count = _tail;
    while (i--) {
      callback(&_array[_index[_count]]);
      _incCount();
    }
  }

  // Удалить несоответствующие элементы
  void filter(bool callback(T *))
  {
    I i = _size;
    _count = _tail;
    while (i--)
      if (!callback(&_array[_index[_count]])) _erase();
      else _incCount();
  }

private:
  I _count;
  inline void _incCount() { _count = ++_count == _size ? 0 : _count; }
  inline void _incHead() { _head = ++_head == _size ? 0 : _head; }
  inline void _decHead() { _head = _head == 0 ? _size - 1 : _head - 1; }
  inline void _incTail() { _tail = ++_tail == _size ? 0 : _tail; }
  inline void _decTail() { _tail = _tail == 0 ? _size - 1 : _tail - 1; }

  // Индекс элемента в положение индекса в буфере
  // Возвращает _count
  void _seek(I index)
  {
    _count = _tail;
    while (_count != _head) {
      if (_index[_count] == index) break;
      _incCount();
    }
  }

  // Удаляет элемент из массива по адресу индекса в массиве
  // Принимает _count
  void _erase()
  {
    I tmp = _count,
      index = _index[_count],
      j;
    while (_count != _head) {
      j = _count;
      _incCount();
      _index[j] = _index[_count]; // Сдвиг индексации
    }
    _index[_count] = index;
    _decHead();
    _heap++;
    _count = tmp;
  }
};
