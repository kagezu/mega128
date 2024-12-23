#include <Arduino.h>
#include <type/array.h>
#include <macros/attribute.h>

#define TASK_STACK_SIZE   200
#define TASK_MAX_COUNT    10

static Array<word, byte> mem(TASK_MAX_COUNT);
static volatile word max_mem = 0x200;

class Task {
private:
  volatile word _sp = 0;
  byte _pid;

public:
  Task()
  {
    if (max_mem != 0x200) return;
    for (byte i = 0; i < TASK_MAX_COUNT; i++) {
      mem.push(max_mem);
      max_mem += TASK_STACK_SIZE;
    }
    mem.clear();
  }

  void create(word size = TASK_STACK_SIZE) __attribute__((noinline))
  {
    _pid = mem.add();
    _sp = *mem.head() - 1 + size;
  }
  inline void erase() { mem.erase(_pid); }
  inline void save() GCC_INLINE { _sp = SP; }
  inline void load() GCC_INLINE
  {
    byte sreg = SREG;
    cli();
    SP = _sp;
    SREG = sreg;
  }
};
