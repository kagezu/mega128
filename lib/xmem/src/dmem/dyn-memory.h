#include "memory-block.h"
#include "type/stack.h"

#define DYN_ERROR     0
#define DYN_OK        1
#define DYN_RECIRCLE  2
#define DYN_COLLECT   3


class DynMemory {
protected:
  word    _start;
  // word    _over;
  Stack<MemoryBlock, byte>   _stack;

public:
  DynMemory() : _stack(0) {}
  DynMemory(word start, word length);

public:
  word  getSizeHeap() { return _stack.peek()->getSize(); }
  word  getSizeFree();

public:
  uint8_t   get(void **var, word size);
  void      free(void **var);
  void      free(void *var);

private:
  static word _count;
  static void _sum(MemoryBlock *block)
  {
    if (block->getLink())
      _count += block->getSize() + sizeof(MemoryBlock);
  }
};
