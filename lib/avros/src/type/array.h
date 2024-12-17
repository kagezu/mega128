#include "buffer.h"

template <typename T, typename  S>
class Array : protected Buffer<S, S> {
protected:
  T *_array;

public:
  Array(S size) :Buffer<S, S>::Buffer(size)
  {
    _array = (T *)malloc(size * sizeof(T));
    clear();
  }
  ~Array()
  {
    free(_array);
  }

  // Очищает массив
  inline void clear()
  {
    Buffer<S, S>::_head = S(0);
    for (S i = 0; i < Buffer<S, S>::_size; i++) Buffer<S, S>::_buffer[i] = i;
  }

protected:
  S isIndex(S index)
  {
    S i;
    for (i = 0; i < Buffer<S, S>::_head; i++)
      if (Buffer<S, S>::_buffer[i] == index) break;
    if (i == Buffer<S, S>::_head) i = -1;             // Индекс не найден
    return i;
  }

public:

  __attribute__((noinline))
    S add(T data)
  {
    if (Buffer<S, S>::_head == Buffer<S, S>::_size) return S(-1);   // Массив полон
    S index = Buffer<S, S>::_buffer[Buffer<S, S>::_head++];
    _array[index] = data;
    return index;
  }
  __attribute__((noinline))
    T get(S index)
  {
    S i, j;
    for (i = 0; i < Buffer<S, S>::_head; i++)
      if (Buffer<S, S>::_buffer[i] == index) break;
    if (i == Buffer<S, S>::_head) return T(0);        // Индекс не найден
    for (j = i + 1; j < Buffer<S, S>::_head; i++, j++)
      Buffer<S, S>::_buffer[i] = Buffer<S, S>::_buffer[j];            // Сдвигаем, закрывая дырку
    Buffer<S, S>::_buffer[i] = index;                  // Сохраняем незанятый индекс
    Buffer<S, S>::_head--;                            // Двигаем голову стека индексов
    return _array[index];
  }

  T at(S index)
  {
    S i;
    for (i = 0; i < Buffer<S, S>::_head; i++)
      if (Buffer<S, S>::_buffer[i] == index) break;
    if (i == Buffer<S, S>::_head) return T(0);        // Индекс не найден
    return _array[index];
  }

  S find(T data)
  {
    return -1;
  }
};
