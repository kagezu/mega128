#include <Arduino.h>
#include <type/array.h>
#include <macros/attribute.h>
#include <macros/context.h>
#include <timer.h>
#include "task.h"

#define TASK_MAX_COUNT      10
#define FREQ_DEFAULT        250

namespace Core {
  void async(void callback()) GCC_NO_INLINE GCC_NAKED;

  static void await(byte) GCC_NO_INLINE;
  static void nextTask() GCC_NAKED GCC_NO_INLINE;

  static  Array<Task, byte> _tasks(TASK_MAX_COUNT);
  static inline void  create() { _tasks.push(Task()); _tasks.head()->create(); }
  static inline void  erase() { _tasks.pop().erase(); }
  static inline Task *current() { return _tasks.head(); }
  static inline Task *next() { return _tasks.circ(); }

  // Частота от (F_CPU / 1024) до (F_CPU / 1024) Гц
  // для 16 МГц от 15625 до 61 Гц
  void init(word freq = FREQ_DEFAULT)
  {
    _tasks.push(Task());
    current()->create(100);
    T0_DIV_1024;
    T0_CTC;
    OCR0A = ((F_CPU / 1024) / freq - 1);
    // T0_COMPA_ON;
  }

  void async(void callback())
  {
    SAVE_CONTEXT;
    // current()->save();
    Core::current()->sp = SP;
    create();
    // current()->load();
    SP = Core::current()->sp;
    sei();
    callback();
    cli();
    erase();
    // current()->load();
    SP = Core::current()->sp;
    LOAD_CONTEXT;
  }


  void nextTask()
  {
    SAVE_CONTEXT;
    // current()->save();
    Core::current()->sp = SP;
    // next()->load();
    SP = Core::next()->sp;
    LOAD_CONTEXT;
  }

  void await(byte limit = 1) { while (_tasks.length() > limit) nextTask(); }

}
// extern void realtime() {}

ISR(TIMER0_COMPA_vect, GCC_NAKED)
{
  SAVE_CONTEXT;
  // current()->save();
  Core::current()->sp = SP;
  // SP = RAMEND;

  // real time func
  // realtime();

  // Диспетчер задач

  // next()->load();
  SP = Core::next()->sp;
  LOAD_CONTEXT;
}
