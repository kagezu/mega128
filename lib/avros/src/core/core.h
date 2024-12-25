#include <Arduino.h>
#include <type/array.h>
#include <macros/attribute.h>
#include <macros/context.h>
#include <timer.h>
#include "task.h"

#define FREQ_DEFAULT    1000


namespace Core {

  Dispatch task;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
  // Частота от (F_CPU / 1024) до (F_CPU / 1024) Гц
  // для 16 МГц от 15625 до 61 Гц
  void GCC_INLINE init()
  {
    task.create();
    task.load();
    T0_DIV_1024;
    T0_CTC;
    OCR0A = ((F_CPU / 1024) / FREQ_DEFAULT - 1);
    T0_COMPA_ON;
  }
#pragma GCC diagnostic pop

  void GCC_NO_INLINE GCC_NAKED async(void callback())
  {
    SAVE_CONTEXT;
    task.save();
    // SP = RAMEND;
    task.create()->load();
    sei();
    callback();
    cli();
    task.erase();
    task.load();
    LOAD_CONTEXT;
  }


  void GCC_NO_INLINE GCC_NAKED nextTask()
  {
    SAVE_CONTEXT;
    task.save();
    task.next()->load();
    LOAD_CONTEXT;
  }

  void GCC_NO_INLINE await(byte limit = 1) { while (task.count() > limit) nextTask(); }

}

ISR(TIMER0_COMPA_vect, GCC_NAKED)
{
  SAVE_CONTEXT;
  Core::task.save();
  // SP = RAMEND;

  // real time func


  // Диспетчер задач

  Core::task.next()->load();
  LOAD_CONTEXT;
}
