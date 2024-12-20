#include "dyn-memory.h"

DynMemory::DynMemory(word start, word length)
  :_stack(start + length)
{
  _start = start;
  _stack.push(MemoryBlock(start, length - sizeof(MemoryBlock)));
}

word DynMemory::getSizeFree()
{
  _count = 0;
  _stack.forEach(this->_sum);
  return (word)_stack.head() - _start - _count;
}

uint8_t DynMemory::get(void **var, word size)
{
  /*
  // Если нет места в куче, либо уже нет самой кучи
  if (size > _stack.peek()->getSize() - sizeof(MemoryBlock) || _stack.peek()->getLink())
    for (MemoryBlock *p = _stack; p != (MemoryBlock *)_over; p--) {
      // Если есть ранее высвобожденный блок с доситочным местом
      if (p->getSize() >= size && !p->getLink()) {
        p->setLink(*(word *)var);
        // если новый размер меньше, происходит утечка памяти
        p->setSize(size);
        // признак повторного использования памяти
        return DYN_RECIRCLE;
      }
      // свободного блока нужного размера не нашлось
      return DYN_ERROR;
    }
*/

  word newBlock = _stack.top()->getStart() + size;
  _stack.top()->setLink(*(word *)var);
  _stack.top()->setSize(size);

  // переопределение адреса и размера кучи
  _stack.push(MemoryBlock(newBlock, (word)_stack.head() - newBlock));
  return DYN_OK;
}

void DynMemory::free(void **var)
{
  // for (MemoryBlock *p = _stack; p != (MemoryBlock *)_over; p--)
    // Ищем указатель в стеке и удаляем его
    // if (p->getLink() == *(word *)var) {
      // p->free();
      // return;
    // }
}

void DynMemory::free(void *var)
{
  // for (MemoryBlock *p = _stack; p != (MemoryBlock *)_over; p--)
    // Ищем указатель в стеке и удаляем его
    // if (p->getStart() == (word)var) {
      // p->free();
      // return;
    // }
}
