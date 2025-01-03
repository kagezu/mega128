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
  Memory(uint16_t start, uint16_t length);

public:
  uint16_t  heap();
  byte *malloc(uint16_t size);
  void  malloc(void **var, uint16_t size);
  void  free(void **var);
  void  free(void *var);
  void  collect();

private:
  static uint16_t _var;
  static MemoryBlock *_ptr;

  MemoryBlock *_find(uint16_t size);

  static void _sum(MemoryBlock *block) { _var += block->is_used() ? 0 : block->size(); }
  static void _near(MemoryBlock *block)
  {
    if (!block->is_used() && block->size() >= _var) {
      if (_ptr) _ptr = block->size() < _ptr->size() ? block : _ptr;
      else _ptr = block;
    }
  }
  static bool _find_link(MemoryBlock *block) { return block->cmp_link(_var); }
  static bool _find_start(MemoryBlock *block) { return block->cmp_start(_var); }

  void _union(byte index)
  {
    byte i = index - 1;
    MemoryBlock *ptr = _stack.at(i);
    if (ptr && !ptr->is_used()) {
      ptr->size(ptr->size() + _stack.at(index)->size());
      _stack.erase(index--);
      _stack.head()->size(_stack.head()->size() + sizeof(MemoryBlock)); // Возвращаем память стека
    }
    i = index + 1;
    ptr = _stack.at(i);
    if (ptr && !ptr->is_used()) {
      _stack.at(index)->size(ptr->size() + _stack.at(index)->size());
      _stack.erase(i);
      _stack.head()->size(_stack.head()->size() + sizeof(MemoryBlock)); // Возвращаем память стека
    }
  }
};
