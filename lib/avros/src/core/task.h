#include <Arduino.h>
#include <macros/attribute.h>

#define TASK_STACK_SIZE   500

class Task {
private:
public:
  volatile word sp = 0;
  void *_context = nullptr;

  Task() {}

  void create(word size = TASK_STACK_SIZE)
  {
    _context = malloc(size);
    if(_context)
    sp = ((word)_context) - 1 + size;
    else
    sp = SP;
  }
  inline void erase() { free(_context); }
  inline void save() GCC_INLINE { sp = SP; }
  inline void load() GCC_INLINE
  {
    byte sreg = SREG;
    cli();
    SP = sp;
    SREG = sreg;
  }
};
