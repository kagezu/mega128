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
private:
  word _sp = 0;
  void *_context = nullptr;

public:
  Task() {}

  void create()
  {
    _context = malloc(TASK_STACK_SIZE);
    _sp = (word)_context - 1 + TASK_STACK_SIZE;
  }

  void erase()
  {
    free(_context);
  }

  void load() GCC_INLINE
  {
    byte sreg = SREG;
    cli();
    SP = _sp;
    SREG = sreg;
  }

  void save() GCC_INLINE
  {
    _sp = SP;
  }
};


class AVROS {
protected:
  Array<Task, byte> _tasks;
public:

  Task *task() { return _tasks.head(); }
  Task *next() { return _tasks.circ(); }

public:
  AVROS() :_tasks(TASK_MAX_COUNT)
  {
    core._tasks.push(Task());
    T0_DIV_1024;
    T0_CTC;
    OCR0A = F_CPU / 1024 / 150 - 1; // 150 Hz
    T0_COMPA_ON;
  }

  void async(void callback()) GCC_NO_INLINE
  {
    SAVE_CONTEXT;
    __CLI;
    _tasks.head()->save();
    _tasks.push(Task());
    _tasks.head()->create();
    _tasks.head()->load();
    __SEI;
    callback();
    __CLI;
    _tasks.pop().erase();
    _tasks.head()->load();
    LOAD_CONTEXT;
    __RETI;
  }

  void await() GCC_NO_INLINE
  {
    __CLI;
    while (_tasks.length() != 1)
      wait();
    __SEI;
  }

private:
  void wait() GCC_NAKED GCC_NO_INLINE
  {
    SAVE_CONTEXT;
    core.task()->save();
    core.next()->load();
    LOAD_CONTEXT;
    __RETI;
  }

} core;

ISR(TIMER0_COMPA_vect, GCC_NAKED)
{
  SAVE_CONTEXT;
  core.task()->save();
  SP = 0x8FF;

  // real time func


  // Диспетчер задач

  core.next()->load();
  LOAD_CONTEXT;
}
