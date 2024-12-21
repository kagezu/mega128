#include <Arduino.h>
#include <type/array.h>
#include <macros/attribute.h>
#include <macros/context.h>
#include <timer.h>
#include "task.h"

#define TASK_MAX_COUNT      10
#define FREQ_DEFAULT        150

static void nextTask() GCC_NAKED GCC_NO_INLINE;

class AvrOS {
protected:
  Array<Task, byte> _tasks;

public:
  // Частота от (F_CPU / 1024) до (F_CPU / 1024) Гц
  // для 16 МГц от 15625 до 61 Гц
  AvrOS(word freq = FREQ_DEFAULT) :_tasks(TASK_MAX_COUNT)
  {
    // _tasks.push(Task());
    // create();
    T0_DIV_1024;
    T0_CTC;
    OCR0A = ((F_CPU / 1024) / 150 - 1);
    T0_COMPA_ON;
  }

  inline void  create() { _tasks.push(Task()); _tasks.head()->create(); }
  inline void  erase() { _tasks.pop().erase(); }
  inline Task *current() { return _tasks.head(); }
  inline Task *next() { return _tasks.circ(); }

  void async(void callback()) //GCC_NAKED
  {
    SAVE_CONTEXT;
    current()->save();
    create();
    // sei();
    // callback();
    // cli();
    // erase();
    // current()->load();
    // LOAD_CONTEXT;
    // sei();
  // asm volatile ("reti");
  }

  void await(byte limit = 1) GCC_NO_INLINE
  {
    while (_tasks.length() > limit) nextTask();
  }
};

extern AvrOS core;
extern void realtime() {}

void nextTask()
{
  SAVE_CONTEXT;
  core.current()->save();
  core.next()->load();
  LOAD_CONTEXT;
}

ISR(TIMER0_COMPA_vect, GCC_NAKED)
{
  SAVE_CONTEXT;
  core.current()->save();
  // SP = RAMEND;

  // real time func
  // realtime();

  // Диспетчер задач

  core.next()->load();
  LOAD_CONTEXT;
  asm volatile ("reti");
}
