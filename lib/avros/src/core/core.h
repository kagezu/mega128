#include <Arduino.h>
#include <type/array.h>
#include <macros/attribute.h>
#include <macros/context.h>
#include <timer.h>
#include "task.h"

#define FREQ_DEFAULT    100

namespace Core {
  void async(void callback()) GCC_NO_INLINE GCC_NAKED;

  void await(byte) GCC_NO_INLINE;
  void nextTask()  GCC_NO_INLINE;

  static  Array<Task, byte> _tasks(TASK_MAX_COUNT);
  static inline Task *current() { return _tasks.head(); }
  static inline Task *next() { return _tasks.circ(); }

  // Частота от (F_CPU / 1024) до (F_CPU / 1024) Гц
  // для 16 МГц от 15625 до 61 Гц
  void init(word freq = FREQ_DEFAULT)
  {
    _tasks.push();
    _tasks.head()->create();
    T0_DIV_1024;
    T0_CTC;
    OCR0A = ((F_CPU / 1024) / freq - 1);
    T0_COMPA_ON;
  }

  void async(void callback())
  {
    SAVE_CONTEXT;
    current()->save();
    // SP = RAMEND;
    _tasks.add();
    // _tasks.push();
    _tasks.head()->create();
    current()->load();
    sei();
    callback();
    cli();
    _tasks.pop().erase();
    current()->load();
    LOAD_CONTEXT;
  }


  void nextTask()
  {
    SAVE_CONTEXT;
    current()->save();
    next()->load();
    LOAD_CONTEXT;
  }

  void await(byte limit = 1) { while (_tasks.length() > limit) nextTask(); }

}

ISR(TIMER0_COMPA_vect, GCC_NAKED)
{
  SAVE_CONTEXT;
  Core::current()->save();
  SP = RAMEND;

  // real time func


  // Диспетчер задач

  Core::next()->load();
  LOAD_CONTEXT;
}
