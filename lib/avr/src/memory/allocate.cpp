#include "allocate.h"

extern int __bss_end;
Memory memory((int)&__bss_end, RAMEND - (int)&__bss_end);

void *malloc(size_t __size)
{
  return memory.malloc(__size);
}

void free(void *__ptr)
{
  memory.free(__ptr);
}
