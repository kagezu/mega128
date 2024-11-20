#include "dyn-memory.h"

uint16_t DynMemory::getSizeFree()
{
  uint16_t sum = 0;
  for (MemoryBlock *p = _stack; p != (MemoryBlock *)_over; p--)
    if (p->getLink() == 0)
      sum += p->getSize();
  return sum;
}

uint8_t DynMemory::get(void **var, uint16_t size)
{
  if (size > _stack->getSize()) return 0;
  uint16_t rise = _stack->getSize() - size;
  _stack->setLink(*var);
}
