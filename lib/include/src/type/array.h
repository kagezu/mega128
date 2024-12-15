#include <Arduino.h>

template <typename T, typename  S>
class Array {
protected:
  T *_array;
  S *_index;
  S _size;                              // Максимальный размер
  S _head;                              // Вершина стека

public:
  Array(S size)
  {
    _array = (T *)malloc(size * sizeof(T));
    _index = (S *)malloc(size * sizeof(S));
    _size = size;
    clear();
  }
  ~Array()
  {
    free(_index);
    free(_array);
  }

protected:
  S isIndex(S index)
  {
    S i;
    for (i = 0; i < _head; i++)
      if (_index[i] == index) break;
    if (i == _head) i = -1;             // Индекс не найден
    return i;
  }

public:
  // Очищает массив
  inline void clear()
  {
    _head = S(0);
    for (S i = 0; i < _size; i++) _index[i] = i;
  }

  S add(T data)
  {
    if (_head == _size) return S(-1);   // Массив полон
    S index = _index[_head++];
    _array[index] = data;
    return index;
  }

  T get(S index)
  {
    S i, j;
    for (i = 0; i < _head; i++)
      if (_index[i] == index) break;
    if (i == _head) return T(0);        // Индекс не найден
    for (j = i + 1; j < _head; i++, j++)
      _index[i] = _index[j];            // Сдвигаем, закрывая дырку
    _index[i] = index;                  // Сохраняем незанятый индекс
    _head--;                            // Двигаем голову стека индексов
    return _array[index];
  }

  T at(S index)
  {
    S i;
    for (i = 0; i < _head; i++)
      if (_index[i] == index) break;
    if (i == _head) return T(0);        // Индекс не найден
    return _array[index];
  }

  S find(T data)
  {
    return -1;
  }
};
