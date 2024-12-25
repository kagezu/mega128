#include <Arduino.h>
#include <type/array.h>
#include <macros/attribute.h>

#define TASK_STACK_SIZE   150
#define TASK_MAX_COUNT    10

struct Task {
  word sp;
  word start;
  word end;
};


class Dispatch {
private:
  Array<Task, byte> tasks;
  byte memory[TASK_MAX_COUNT][TASK_STACK_SIZE];

public:
  Dispatch() :tasks(TASK_MAX_COUNT)
  {
    for (byte i = 0; i < TASK_MAX_COUNT; i++) {
      Task t = { 0, (word)memory[i],(word)memory[i + 1] - 1 };
      tasks.write(t);
    }
    tasks.clear();
  }

  inline Task *curent() { return tasks.tail(); }
  inline byte count() { return tasks.length(); }

  inline Dispatch *create()
  {
    // Деление происходит, только если есть память
    // Иначе деление на задачи не происходит
    Task *task = tasks.unshift();
    if (task) task->sp = task->end;
    return this;
  }
  inline Dispatch *next() { tasks.circ(); return this; }
  inline Dispatch *erase() { tasks.shift(); return this; }
  inline void GCC_INLINE save() GCC_INLINE { tasks.tail()->sp = SP; }
  inline void GCC_INLINE load() GCC_INLINE
  {
    byte sreg = SREG;
    cli();
    SP = tasks.tail()->sp;
    SREG = sreg;

    // asm volatile (
    //   "in __tmp_reg__, __SREG__   \n"
    //   "sei                        \n"
    //   "mov r28, %A0               \n"
    //   "mov r29, %B0               \n"
    //   "out __SP_L__, r28          \n"
    //   "out __SP_H__, r29          \n"
    //   "out __SREG__, __tmp_reg__  \n"
    //   ::"r"(_sp));
  }
};
