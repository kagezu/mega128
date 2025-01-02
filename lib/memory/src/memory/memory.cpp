#include <macros/helper.h>
#include "memory.h"

Memory::Memory(uint16_t start, uint16_t length)
  :_stack(start + length)
{
  _start = start;
  _stack.push()->init(start, length - 2 * sizeof(MemoryBlock));
}

uint16_t Memory::get_heap()
{
  I_SAVE;
  _var = 0;
  _stack.each(this->_max);
  I_REST;
  return _var;
}

void Memory::get(void **var, uint16_t size)
{
  I_SAVE;
  uint16_t heap = _stack.head()->get_size();
  uint16_t start = _stack.head()->get_start() + size;
  if (size + sizeof(MemoryBlock) > heap) throw PSTR("Memory::get() error");
  _stack.head()->set_link((uint16_t)var);
  _stack.head()->set_size(size);
  _stack.push()->init(start, heap - size - sizeof(MemoryBlock));
  I_REST;
}

void *Memory::get(uint16_t size)
{
  I_SAVE;
  uint16_t heap = _stack.head()->get_size();
  uint16_t start = _stack.head()->get_start() + size;
  if (size + sizeof(MemoryBlock) > heap) throw PSTR("Memory::get() error");
  _stack.head()->use();
  _stack.head()->set_size(size);
  _stack.push()->init(start, heap - size - sizeof(MemoryBlock));
  I_REST;
  return (void *)start;
}

void Memory::free(void **var)
{
  I_SAVE;
  _var = (uint16_t)var;
  byte index = _stack.findindex(this->_find_link);
  if (index) {
    _stack.at(index)->free();
    _union(index);
  }
  I_REST;
}

void Memory::free(void *var)
{
  I_SAVE;
  _var = (uint16_t)var;
  byte index = _stack.findindex(this->_find_start);
  if (index) {
    _stack.at(index)->free();
    _union(index);
  }
  I_REST;
}
