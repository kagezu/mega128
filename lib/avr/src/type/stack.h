#pragma once
#include <Arduino.h>

/*
#### Stack<T, I> T - element, I - index
+ clear() / size()
+ push(T) --> [ sp ]
+ *pop () <-- [++sp]
+ *head() <== [sp+1]
+ *top () <== [ sp ]
+ each() / find() / findindex()
+ erase(I) / insert_next(T*) / at(I)
*/
template <typename T, typename I>
class Stack {
protected:
  T *_stack;
  I _size = 0;

public:
  Stack(uint16_t sp) : _stack((T *)(sp - sizeof(T))) {}

  void clear() { _stack -= _size; _size = 0; }
  I size() { return _size; }
  void push(T data) { *_stack-- = data; _size++; }
  T *push() { T *ptr = _stack--; _size++; return ptr; }
  T *pop() { _size--; return ++_stack; }
  T *top() { return _stack; }
  T *head() { return _stack + 1; }

public:
  void each(void callback(T *ptr))
  {
    I index = _size;
    T *ptr = _stack;
    while (index--) callback(++ptr);
  }

  I findindex(bool callback(T *ptr))
  {
    I index = _size;
    T *ptr = _stack;
    while (index) {
      if (callback(++ptr)) break;
      index--;
    }
    return index;
  }

  T *find(bool callback(T *ptr))
  {
    I index = _size;
    T *ptr = _stack;
    while (index) {
      if (callback(++ptr)) break;
      index--;
    }
    return index ? ptr : nullptr;
  }

  void erase(I index)
  {
    if (index > _size || index == 0) return;
    T *ptr = _stack + _size - index + 1;
    index -= _size;
    while (index++) {
      *ptr = *(ptr - 1);
      ptr--;
    }
    _stack++;
    _size--;
  }

  T *insert_post(T *ptr)
  {
    T *new_element = ptr - 1;
    ptr = _stack;
    while (ptr != new_element) {
      *ptr = *(ptr + 1);
      ptr++;
    }
    _stack--;
    _size++;
    return new_element;
  }

  T *at(I index)
  {
    if (index > _size || index == 0) return nullptr;
    return _stack + _size - index + 1;
  }

};
