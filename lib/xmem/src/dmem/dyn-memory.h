#include "memory-block.h"

class DynMemory {

protected:
  uint16_t _start;
  uint16_t _over;
  MemoryBlock *_stack;

public:
  DynMemory(uint16_t start, uint16_t length)
  {
    _start = start;
    _over = start + length;
    _stack = (MemoryBlock *)(_over - sizeof(MemoryBlock));
    _stack->init(start, length);
  }

public:
  uint16_t getSizeHeap();
  uint16_t getSizeFree();
  uint8_t *getFromHeap(uint16_t size);
  uint8_t *getFree(uint16_t size);
  void free(uint8_t &&ptr);
  void collect();

};
