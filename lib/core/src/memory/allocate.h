#pragma once
#include "memory.h"

extern Memory memory;
void *malloc(size_t __size)  __attribute__((__malloc__));
void free(void *__ptr);


extern void *calloc(size_t __nele, size_t __size) __attribute__((__malloc__));
extern void *realloc(void *__ptr, size_t __size) __attribute__((__malloc__));

