#include <avr.h>
#include <macros/attribute.h>

class Task {
protected:
  volatile uint16_t _sp;
  uint16_t _start;
  uint16_t _end;

public:
  Task(uint16_t start, uint16_t size)
    :_sp(0), _start(start), _end(start + size - 1)
  {}

  GCC_INLINE void clear() { _sp = _end; }
  GCC_INLINE void load() { SP = _sp; }
  GCC_INLINE void save() { _sp = SP; }
  GCC_INLINE void deep_load()
  {
    asm volatile (
      "pop r0                     \n"
      "pop r1                     \n"
      "out __SP_L__, A0           \n"
      "out __SP_H__, B0           \n"
      "push r1                    \n"
      "push r0                    \n"
      "clr  r1                    \n"
      ::"r"(_sp)
      );
  }
  GCC_INLINE void deep_save()
  {
    asm volatile (
      "pop r0                     \n"
      "pop r1                     \n"
      "in  A0, __SP_L__,          \n"
      "in  B0, __SP_H__,          \n"
      "push r1                    \n"
      "push r0                    \n"
      "clr  r1                    \n"
      :"=r"(_sp));
  }
};
