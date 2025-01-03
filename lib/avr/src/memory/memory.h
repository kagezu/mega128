#include "memory-block.h"
#include "type/stack.h"

#define MEM_BLOCK_MIN_SIZE    16


/**
 #### Memory(uint16_t start, uint16_t length)
 */
class Memory {
public:
  // protected:
  uint16_t                  _start;
  Stack<MemoryBlock, byte>  _stack;

public:
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
  static MemoryBlock *_ptr;
  static void _sum(MemoryBlock *block) { _var += block->is_used() ? 0 : block->get_size(); }
  static void _near(MemoryBlock *block)
  {
    if (!block->is_used() && block->get_size() >= _var) {
      if (_ptr) _ptr = block->get_size() < _ptr->get_size() ? block : _ptr;
      else _ptr = block;
    }
  }
  static bool _find_link(MemoryBlock *block) { return block->is_link(_var); }
  static bool _find_start(MemoryBlock *block) { return block->is_start(_var); }

  byte _union(byte index)
  {
    byte deleted = 0;
    byte i = index - 1;
    MemoryBlock *ptr = _stack.at(i);
    if (ptr && !ptr->is_used()) {
      ptr->set_size(ptr->get_size() + _stack.at(index)->get_size());
      _stack.erase(index--);
      deleted++;
    }
    i = index + 1;
    ptr = _stack.at(i);
    if (ptr && !ptr->is_used()) {
      _stack.at(index)->set_size(ptr->get_size() + _stack.at(index)->get_size());
      _stack.erase(i);
      deleted++;
    }
    return deleted;
  }
};
