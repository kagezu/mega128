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
  _var = size;
  _ptr = nullptr;
  _stack.each(this->_near);
  if (_ptr == nullptr) throw PSTR("Memory::get() error");
  uint16_t heap = _ptr->get_size();
  uint16_t start = _ptr->get_start() + size;
  _ptr->set_link((uint16_t)var);
  _ptr->set_size(size);
  if (_ptr == _stack.head())
    _stack.push()->init(start, heap - size - sizeof(MemoryBlock));
  I_REST;
}

void *Memory::get(uint16_t size)
{
  I_SAVE;
  _var = size;
  _ptr = nullptr;
  _stack.each(this->_near);
  if (_ptr == nullptr) throw PSTR("Memory::get() error");
  uint16_t heap = _ptr->get_size();
  uint16_t start = _ptr->get_start() + size;
  _stack.head()->use();
  _ptr->set_size(size);
  if (_ptr == _stack.head())
    _stack.push()->init(start, heap - size - sizeof(MemoryBlock));
  I_REST;
  return (void *)_ptr->get_start();
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
