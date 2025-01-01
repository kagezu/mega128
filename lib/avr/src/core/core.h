#include <type/array.h>
#include <macros/attribute.h>
#include <macros/context.h>
#include <timer.h>
#include "task.h"

#define TASK_STACK_SIZE   250
#define TASK_MAX_COUNT    3
#define FREQ_DEFAULT      300

#pragma GCC push_options
#pragma GCC optimize "O0"
namespace Core {
  static byte memory[TASK_MAX_COUNT][TASK_STACK_SIZE];
  static Array<Task, byte> tasks(TASK_MAX_COUNT);

  // Количество запущенных задач включая main
  // но без учёта прерываний
  inline byte count() { return tasks.length(); }

  // Текущая задача
  inline Task *current() { return tasks.tail(); }

  // Переключение на следующую задачу
  inline Task *next() { return tasks.back(); }

  // Завершить текущую задачу
  inline void drop() { tasks.shift(); }

  // Начать новую задачу
  inline Task *now()
  {
    // Деление происходит, только если есть память
    // Иначе деление на задачи не происходит
    Task *task = tasks.unshift();
    if (task != nullptr) task->clear();
    return task;
  }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
  // Инициализация операционной системы
  // Частота от (F_CPU / 1024) до (F_CPU / 1024) Гц
  // для 16 МГц от 15625 до 61 Гц
  void GCC_INLINE init()
  {
    for (byte i = 0; i < TASK_MAX_COUNT; i++) {
      Task t((uint16_t)memory[i], TASK_STACK_SIZE);
      tasks.write(t);
    }
    tasks.clear();
    now()->load();
    T0_DIV_1024;
    T0_CTC;
    OCR0A = ((F_CPU / 1024) / FREQ_DEFAULT - 1);
    T0_COMPA_ON;
  }
#pragma GCC diagnostic pop

  void GCC_NO_INLINE GCC_NAKED next_task()
  {
    SAVE_CONTEXT;
    current()->save();
    next()->load();
    LOAD_CONTEXT;
  }

  // Ждать завершения асинхронных функций
  void GCC_NO_INLINE await(byte limit = 1)
  {
    while (count() - limit)
      next_task();
  }

  // Выполнить функцию асинхронно
  void GCC_NO_INLINE GCC_NAKED async(void callback())
  {
    while (!tasks.heap())
      next_task();
    SAVE_CONTEXT;
    current()->save();
    now()->load();
    sei();
    callback();
    cli();
    drop();
    current()->load();
    LOAD_CONTEXT;
  }

  // Выполнить функцию асинхронно с аргументами
  template <typename T>
  void GCC_NO_INLINE GCC_NAKED async(void callback(T), T arg)
  {
    while (!tasks.heap())
      next_task();
    SAVE_CONTEXT;
    current()->save();
    now()->load();
    sei();
    callback(arg);
    cli();
    drop();
    current()->load();
    LOAD_CONTEXT;
  }
}
#pragma GCC pop_options

GCC_INIT(7)
{
  Core::init();
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
