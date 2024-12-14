#include <Arduino.h>

template <typename T, typename  S>
class Array {
protected:
  T *_array;
  S *_index;
  S _size;    // Максимальный размер
  S _head;    // Вершина стека

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

  // Очищает массив
  inline void clear()
  {
    _head = S(0);
    for (S i = 0; i < _size; i++) _index[i] = i;
  }

public:
  S add(T data)
  {
    if (_head == _size) return -1;      // Массив полон
    S index = _index[_head++];
    _array[index] = data;
    return index;
  }

  T get(S index)
  {
    S i;
    for (i = 0; i < _head; i++)
      if (_index[i] == index) break;
    for (S j = _head; j > i; j--) {
      S t = _index[j--];
      _index[j] = t;
    }

    return _array[index];
  }
};
