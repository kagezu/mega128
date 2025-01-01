#include "memory-block.h"
#include "type/stack.h"

#define DYN_ERROR     0
#define DYN_OK        1
#define DYN_RECIRCLE  2
#define DYN_COLLECT   3


class DynMemory {
protected:
  uint16_t    _start;
  // uint16_t    _over;
  Stack<MemoryBlock, byte>   _stack;

public:
  DynMemory() : _stack(0) {}
  DynMemory(uint16_t start, uint16_t length);

public:
  uint16_t  getSizeHeap() { return _stack.top()->getSize(); }
  uint16_t  getSizeFree();

public:
  uint8_t   get(void **var, uint16_t size);
  void      free(void **var);
  void      free(void *var);

private:
  static uint16_t _count;
  static void _sum(MemoryBlock *block)
  {
    if (block->getLink())
      _count += block->getSize() + sizeof(MemoryBlock);
  }
};
