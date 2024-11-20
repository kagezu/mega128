#include "memory-block.h"

class DynMemory {
protected:
  uint16_t    _start;
  uint16_t    _over;
  MemoryBlock *_stack;

public:
  DynMemory() {}
  DynMemory(uint16_t start, uint16_t length)
  {
    _start = start;
    _over = start + length;
    _stack = (MemoryBlock *)_over;
    *(--_stack) = MemoryBlock(start, length);
  }

public:
  uint16_t  getSizeHeap() { return _stack->getSize(); }

public:
  uint16_t  getSizeFree();

  uint8_t   get(void **var, uint16_t size);
  void      free(void **var);
};
