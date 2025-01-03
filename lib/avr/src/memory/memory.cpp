#include <macros/helper.h>
#include "memory.h"

Memory::Memory(uint16_t start, uint16_t length)
  :_stack(start + length)
{
  _start = start;
  _stack.push()->init(start, length - 2 * sizeof(MemoryBlock));
}

uint16_t Memory::_var;
MemoryBlock *Memory::_ptr;

uint16_t Memory::get_heap()
{
  I_SAVE;
  _var = 0;
  _stack.each(this->_sum);
  I_REST;
  return _var;
}

void Memory::get(void **var, uint16_t size)
{
  I_SAVE;
  _var = size;
  _ptr = nullptr;
  _stack.each(this->_near); // Поиск блока наиболее близкого размера
  if (_ptr == nullptr) while (1); // throw PSTR("Memory::get() error"); // Блока для выделения памяти не найдено
  uint16_t heap = _ptr->get_size(); // Свободная память блока
  _ptr->set_link((uint16_t)var); // Сохранение ссылки на указатель
  _ptr->set_size(size); // Новый размер блока
  // Дробим блок, если новый блок больше минимального и есть память на стек
  if (heap > size + MEM_BLOCK_MIN_SIZE && _stack.head()->get_size() > sizeof(MemoryBlock)) {
    _stack.insert_post(_ptr)->init(_ptr->get_start() + size, heap - size - sizeof(MemoryBlock)); // Новый блок
    _stack.head()->set_size(_stack.head()->get_size() - sizeof(MemoryBlock)); // Уменьшаем память блока на вершине стека
  }
  I_REST;
}

void *Memory::get(uint16_t size) // ok ?
{
  I_SAVE;
  _var = size;
  _ptr = nullptr;
  MemoryBlock *top = _stack.pop();
  _stack.each(this->_near);
  _stack.push();
  if (_ptr == nullptr) {
    if (top->get_size() >= size + sizeof(MemoryBlock)) _ptr = top;
    else while (1); // throw PSTR("Memory::get() error");
  }
  uint16_t heap = _ptr->get_size();
  uint16_t start = _ptr->get_start();
  _ptr->use();
  if (heap > size + MEM_BLOCK_MIN_SIZE && top->get_size() > sizeof(MemoryBlock)) {
    _ptr->set_size(size);
    _stack.insert_post(_ptr)->init(start + size, heap - size);
    _stack.head()->set_size(_stack.head()->get_size() - sizeof(MemoryBlock));
  }
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
  if (index && _stack.at(index)->is_used()) {
    _stack.at(index)->free();
    byte del = _union(index);
    if (del)
      _stack.head()->set_size(_stack.head()->get_size() + (sizeof(MemoryBlock) << (del - 1)));
  }
  I_REST;
}
