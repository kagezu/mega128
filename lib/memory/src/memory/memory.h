#include "memory-block.h"
#include "type/stack.h"

class Memory {
protected:
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
  static void _max(MemoryBlock *block) { _var = block->is_used() && block->get_size() > _var ? block->get_size() : _var; }
  static bool _find_link(MemoryBlock *block) { return block->is_link(_var); }
  static bool _find_start(MemoryBlock *block) { return block->is_start(_var); }

  void _union(byte index)
  {
    byte i = index - 1;
    MemoryBlock *ptr = _stack.at(i);
    if (ptr && !ptr->is_used()) {
      ptr->set_size(ptr->get_size() + _stack.at(index)->get_size());
      _stack.erase(index--);
    }
    i = index + 1;
    ptr = _stack.at(i);
    if (ptr && !ptr->is_used()) {
      _stack.at(index)->set_size(ptr->get_size() + _stack.at(index)->get_size());
      _stack.erase(i);
    }
  }
};
