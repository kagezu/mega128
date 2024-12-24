#include <Arduino.h>
#include <type/array.h>
#include <macros/attribute.h>

#define TASK_STACK_SIZE   150
#define TASK_MAX_COUNT    10

static Array<word, byte> mem(TASK_MAX_COUNT);
static byte array[TASK_MAX_COUNT][TASK_STACK_SIZE];
static byte flag = 0;

class Task {
private:
public:
  volatile word _sp = 0;
  word *_pid = 0;

  Task()
  {
    if (flag) return;
    for (byte i = 0; i < TASK_MAX_COUNT; i++) {
      mem.write((word)array[i]);
    }
    mem.clear();
    flag = 1;
  }

  inline Task *create()
  {
    _pid = mem.push();
    _sp = *_pid - 1 + TASK_STACK_SIZE;
    return this;
  }
  inline void erase()
  {
    mem.erase(_pid);
  }
  inline void save() GCC_INLINE { _sp = SP; }
  inline void load() GCC_INLINE
  {
    byte sreg = SREG;
    cli();
    SP = _sp;
    SREG = sreg;
  }
};
