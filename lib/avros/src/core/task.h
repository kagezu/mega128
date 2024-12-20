#include <Arduino.h>
#include <macros/attribute.h>

#define TASK_STACK_SIZE   360

class Task {
private:
public:
  volatile  word _sp = 0;
  void *_context = nullptr;

  Task() {}

  void create(word size = TASK_STACK_SIZE)
  {
    _context = malloc(size);
    _sp = ((word)_context) - 1 + size;
  }
  inline void erase() { free(_context); }
  inline void save() GCC_INLINE { _sp = SP; }
  inline void load() GCC_INLINE
  {
    byte sreg = SREG;
    cli();
    SP = _sp;
    SREG = sreg;
  }
};
