#include <macros/helper.h>
#include "memory.h"

Memory::Memory(uint16_t start, uint16_t length)
  :_stack(start + length)
{
  _stack.push()->init(start, length - 2 * sizeof(MemoryBlock));
  _status = MEM_FREE;
}

uint16_t Memory::_var;
MemoryBlock *Memory::_ptr;

uint16_t Memory::heap()
{
  I_SAVE;
  _var = 0;
  _stack.each(this->_sum);
  I_REST;
  return _var;
}


void Memory::malloc(void **var, uint16_t size)
{
  I_SAVE;
  _find(size)->use((uint16_t)var); // Передаём ссылку на указатель в запись блока
  I_REST;
}

byte *Memory::malloc(uint16_t size)
{
  I_SAVE;
  MemoryBlock *ptr = _find(size);
  ptr->use(); // Указать, что блок используется и заблокирован
  I_REST;
  return (byte *)ptr->start();
}

// void *Memory::malloc(uint16_t size) //  ok
// {
//   _var = size;
//   _ptr = nullptr;
//   MemoryBlock *top = _stack.pop(); // Исключаем из поиска блок на вершине стека
//   _stack.each(this->_near); // Поиск блока наиболее близкого размера
//   _stack.push(); // Восстанавливаем блок на вершине стека
//   if (_ptr == nullptr) {// Блока для выделения памяти не найдено
//     if (top->size() >= size + sizeof(MemoryBlock)) _ptr = top; // Проверяем блок на вершине стека
//     else while (1); // Остановка при невозможности выделить память
//   }
//   uint16_t heap = _ptr->size(); // Свободная память выделяемого блока
//   uint16_t start = _ptr->start(); // Начало  выделяемого блока
//   _ptr->use(); // Указать, что блок используется и заблокирован
//   // Дробим блок, если новый блок будет больше минимального и есть память на стек
//   if (heap > size + MEM_BLOCK_MIN_SIZE && top->size() > sizeof(MemoryBlock)) {
//     _ptr->size(size); // Новый размер блока
//     _stack.insert_post(_ptr)->init(start + size, heap - size); // Новый блок
//     _stack.head()->size(_stack.head()->size() - sizeof(MemoryBlock)); // Уменьшаем память блока на вершине стека
//   }
//   return (void *)start;
// }

void Memory::free(void **var)
{
  I_SAVE;
  _var = (uint16_t)var;
  byte index = _stack.findindex(this->_find_link);
  if (index && _stack.at(index)->is_used()) {
    _stack.at(index)->free();
    byte del = _union(index); // Получаем количество удалённых записей из стека
    if (del)
      _stack.head()->size(_stack.head()->size() + (sizeof(MemoryBlock) << (del - 1))); // Возвращаем память стека
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
    byte del = _union(index); // Получаем количество удалённых записей из стека
    if (del)
      _stack.head()->size(_stack.head()->size() + (sizeof(MemoryBlock) << (del - 1))); // Возвращаем память стека
  }
  I_REST;
}

void Memory::collect()
{
  if (_status) return;

  I_SAVE;
  _var = 0;
  // _stack.each(this->_sum);
  I_REST;
}

MemoryBlock *Memory::_find(uint16_t size)
{
  _var = size;
  _ptr = nullptr;
  MemoryBlock *top = _stack.pop(); // Исключаем из поиска блок на вершине стека
  _stack.each(this->_near); // Поиск блока наиболее близкого размера
  _stack.push(); // Восстанавливаем блок на вершине стека
  if (_ptr == nullptr) {// Блока для выделения памяти не найдено
    if (top->size() >= size + sizeof(MemoryBlock)) _ptr = top; // Проверяем блок на вершине стека
    else while (1); // Остановка при невозможности выделить память
  }
  uint16_t heap = _ptr->size(); // Свободная память выделяемого блока
  uint16_t start = _ptr->start(); // Начало  выделяемого блока
  // Дробим блок, если новый блок будет больше минимального и есть память на стек
  if (heap > size + MEM_BLOCK_MIN_SIZE && top->size() > sizeof(MemoryBlock)) {
    _ptr->size(size); // Новый размер блока
    _stack.insert_post(_ptr)->init(start + size, heap - size); // Новый блок
    _stack.head()->size(_stack.head()->size() - sizeof(MemoryBlock)); // Уменьшаем память блока на вершине стека
  }
  return _ptr;
}
