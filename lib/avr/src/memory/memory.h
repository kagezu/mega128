#pragma once
#include "memory-block.h"
#include "type/stack.h"

#define MEM_BLOCK_MIN_SIZE    16

#define MEM_LOCK      128
#define MEM_FREE      0

/**
 #### Memory(uint16_t start, uint16_t length)
 */
class Memory {
protected:
  byte                      _status;
  Stack<MemoryBlock, byte>  _stack;

public:
  Memory() {}
  Memory(uint16_t start, uint16_t length) { init(start, length); }

public:
  void init(uint16_t start, uint16_t length);
  uint16_t  heap();
  byte *malloc(uint16_t size);
  void  malloc(void **var, uint16_t size);
  void  free(void **var);
  void  free(void *var);
  void  shrink();

private:
  static uint16_t _var;
  static MemoryBlock *_ptr;

  MemoryBlock *_find(uint16_t size);
  void _union(byte index);

  static bool _find_link(MemoryBlock *block) { return block->cmp_link(_var); }
  static bool _find_start(MemoryBlock *block) { return block->cmp_start(_var); }
  static void _sum(MemoryBlock *block) { _var += block->is_used() ? 0 : block->size(); }
  static void _near(MemoryBlock *block)
  {
    if (!block->is_used() && block->size() >= _var) {
      if (_ptr) _ptr = block->size() < _ptr->size() ? block : _ptr;
      else _ptr = block;
    }
  }
};

#ifdef USE_MALLOC
extern Memory memory;
void *malloc(size_t __size)  __attribute__((__malloc__));
void free(void *__ptr);

#endif
