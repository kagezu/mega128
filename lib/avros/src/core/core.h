#include <Arduino.h>
#include <type/array.h>
#include <type/buffer.h>
#include <macros/attribute.h>
#include <macros/context.h>
#include <macros/avrasm.h>
#include <timer.h>
#define TASK_MAX_COUNT      10
#define TASK_STACK_SIZE     100

class Task {
public:
  word sp;
  void *context;

public:
  Task(byte param = 1)
  {
    if (param) {
      context = malloc(TASK_STACK_SIZE);
      sp = (word)context - 1 + TASK_STACK_SIZE;
      SP = sp;
    }
  }
  void erase()
  {
    free(context);
  }
};


class AVROS {
protected:
public:
  Array<Task, byte> _tasks;

public:
  AVROS() :_tasks(TASK_MAX_COUNT)
  {
    T0_DIV_1024;
    T0_CTC;
    OCR0A = F_CPU / 1024 / 150 - 1; // 150 Hz
    T0_COMPA_ON;
  }

  void async(void callback()) GCC_NO_INLINE
  {
    SAVE_CONTEXT;
    _tasks.push(Task());
    __SEI;
    callback();
    __CLI;
    _tasks.pop().erase();
    SP = _tasks.head()->sp;
    LOAD_CONTEXT;
  }
} core;

ISR(TIMER0_COMPA_vect, GCC_NAKED)
{
  SAVE_CONTEXT;
  core._tasks.head()->sp = SP;
  SP = 0x8FF;

  // real time func


  // Диспетчер задач

  Task task = core._tasks.shift();
  core._tasks.push(task);
  SP = task.sp;
  LOAD_CONTEXT;
}
