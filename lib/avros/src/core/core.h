#include <Arduino.h>
#include <type/array.h>
#include <macros/attribute.h>
#include <macros/context.h>
#include <timer.h>
#include "task.h"

#define FREQ_DEFAULT    100

namespace Core {
  volatile void async(void callback()) GCC_NO_INLINE GCC_NAKED;
  void init(word) GCC_NO_INLINE;

  void await(byte) GCC_NO_INLINE;
  void nextTask()  GCC_NO_INLINE GCC_NAKED;

  Array<Task, byte> tasks(TASK_MAX_COUNT);

  // Частота от (F_CPU / 1024) до (F_CPU / 1024) Гц
  // для 16 МГц от 15625 до 61 Гц
  void init()
  {
    Task a;
    a.create();
    tasks.push(a);
    T0_DIV_1024;
    T0_CTC;
    OCR0A = ((F_CPU / 1024) / FREQ_DEFAULT - 1);
    T0_COMPA_ON;
  }

  volatile  void async(void callback())
  {
    SAVE_CONTEXT;
    tasks.head()->save();
    // SP = RAMEND;
    tasks.push();
    tasks.head()->create()->load();
    sei();
    callback();
    cli();
    tasks.pop()->erase();
    tasks.head()->load();
    LOAD_CONTEXT;
  }


  void nextTask()
  {
    SAVE_CONTEXT;
    tasks.head()->save();
    tasks.circ()->load();
    LOAD_CONTEXT;
  }

  void await(byte limit = 1) { while (tasks.length() > limit) nextTask(); }

}

ISR(TIMER0_COMPA_vect, GCC_NAKED)
{
  SAVE_CONTEXT;
  Core::tasks.head()->save();
  // SP = RAMEND;

  // real time func


  // Диспетчер задач

  Core::tasks.circ()->load();
  LOAD_CONTEXT;
}
