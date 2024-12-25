#include <Arduino.h>
#include <type/array.h>
#include <macros/attribute.h>
#include <macros/context.h>
#include <timer.h>
#include "dispatch.h"

#define FREQ_DEFAULT    1000


namespace Core {

  Dispatch task;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
  // Частота от (F_CPU / 1024) до (F_CPU / 1024) Гц
  // для 16 МГц от 15625 до 61 Гц
  void GCC_INLINE init()
  {
    task.now();
    task.current()->load();
    T0_DIV_1024;
    T0_CTC;
    OCR0A = ((F_CPU / 1024) / FREQ_DEFAULT - 1);
    T0_COMPA_ON;
  }
#pragma GCC diagnostic pop

  void GCC_NO_INLINE GCC_NAKED async(void callback())
  {
    SAVE_CONTEXT;
    task.current()->save();
    task.now();
    task.current()->load();
    sei();
    callback();
    cli();
    task.drop();
    task.current()->load();
    LOAD_CONTEXT;
  }

  void GCC_NO_INLINE GCC_NAKED next_task()
  {
    SAVE_CONTEXT;
    task.current()->save();
    task.next()->load();
    LOAD_CONTEXT;
  }

  void GCC_NO_INLINE await(byte limit = 1) { while (task.count() > limit); next_task(); }

}

ISR(TIMER0_COMPA_vect, GCC_NAKED)
{
  SAVE_CONTEXT;
  Core::task.current()->save();
  // SP = RAMEND;

  // real time func


  // Диспетчер задач

  Core::task.next()->load();
  LOAD_CONTEXT;
}
