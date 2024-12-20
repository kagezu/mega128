#include <Arduino.h>
/*
#### Stack<T, I> T - element, I - index
+ clear() / size()
+ push() --> [ sp ]
+ pop () <-- [++sp]
+ peek() <== [sp+1]
+ head() <== [ sp ]
+ forEach() / find() / findindex()
*/
template <typename T, typename I>
class Stack {
protected:
  T *_stack;
  I _size = 0;

public:
  Stack(word sp) : _stack(sp - sizeof(T)) {}
  ~Array() {}

  inline void clear() { _stack -= _size; _size = 0; }
  inline I size() { return _size; }
  inline void push(T data) { *_stack++ = data; _size++; }
  inline  T pop() { _size--; return *_stack--; }
  inline  T *head() { return _stack; }
  inline  T *peek() { return _stack + 1; }

public:
  void forEach(callback(T *ptr))
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
    return ptr;
  }

  void erase(S index)
  {
    if (index > _size || index == 0) return;
    T *ptr = _stack + index;
    while (index--) {
      *ptr = *(ptr - 1);
      ptr--;
    }
    _stack++;
    _size--;
  }

};

Stack<byte, byte> g;
