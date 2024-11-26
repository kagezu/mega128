#include "dyn-memory.h"

DynMemory::DynMemory(uint16_t start, uint16_t length)
{
  _start = start;
  _over = start + length;
  _stack = (MemoryBlock *)_over;
  *(--_stack) = MemoryBlock(start, length - sizeof(MemoryBlock));
}

uint16_t DynMemory::getSizeFree()
{
  uint16_t sum = 0;
  for (MemoryBlock *p = _stack; p != (MemoryBlock *)_over; p--)
    if (p->getLink())
      sum += p->getSize() + sizeof(MemoryBlock);
  return _over - _start - sum;
}

uint8_t DynMemory::get(void **var, uint16_t size)
{
  // Если нет места в куче, либо уже нет самой кучи
  if (size > _stack->getSize() - sizeof(MemoryBlock) || _stack->getLink())
    for (MemoryBlock *p = _stack; p != (MemoryBlock *)_over; p--) {
      // Если есть ранее высвобожденный блок с доситочным местом
      if (p->getSize() >= size && !p->getLink()) {
        p->setLink(*(uint16_t *)var);
        // если новый размер меньше, происходит утечка памяти
        p->setSize(size);
        // признак повторного использования памяти
        return DYN_RECIRCLE;
      }
      // свободного блока нужного размера не нашлось
      return DYN_ERROR;
    }

  uint16_t newBlock = _stack->getStart() + size;
  _stack->setLink(*(uint16_t *)var);
  _stack->setSize(size);
  _stack--;
  // переопределение адреса и размера кучи
  *_stack = MemoryBlock(newBlock, (uint16_t)_stack - newBlock);
  return DYN_OK;
}

void DynMemory::free(void **var)
{
  for (MemoryBlock *p = _stack; p != (MemoryBlock *)_over; p--)
    // Ищем указатель в стеке и удаляем его
    if (p->getLink() == *(uint16_t *)var) {
      p->free();
      return;
    }
}

void DynMemory::free(void *var)
{
  for (MemoryBlock *p = _stack; p != (MemoryBlock *)_over; p--)
    // Ищем указатель в стеке и удаляем его
    if (p->getStart() == (uint16_t)var) {
      p->free();
      return;
    }
}
