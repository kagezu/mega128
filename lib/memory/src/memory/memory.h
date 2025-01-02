#include "memory-block.h"
#include "type/stack.h"

#define MEM_ERROR     0
#define MEM_OK        1
#define MEM_RECIRCLE  2
#define MEM_COLLECT   3


class Memory {
protected:
  uint16_t    _start;
  Stack<MemoryBlock, byte>   _stack;

public:
  Memory() : _stack(0) {}
  Memory(uint16_t start, uint16_t length);

public:
  uint16_t  get_heap();

public:
  void *get(uint16_t size);
  void  get(void **var, uint16_t size);
  void  free(void **var);
  void  free(void *var);

private:
  static uint16_t _var;
  static void _max(MemoryBlock *block)
  {
    if (block->is_used())
      _var = block->get_size() > _var ? block->get_size() : _var;
  }
};
