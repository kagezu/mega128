#include <Arduino.h>
#include <type/array.h>
#include <macros/attribute.h>
#include "task.h"

#define TASK_STACK_SIZE   150
#define TASK_MAX_COUNT    5

class Dispatch {
private:
  Array<Task, byte> tasks;
  byte memory[TASK_MAX_COUNT][TASK_STACK_SIZE];

public:
  Dispatch() :tasks(TASK_MAX_COUNT)
  {
    for (byte i = 0; i < TASK_MAX_COUNT; i++) {
      Task t((word)memory[i], TASK_STACK_SIZE);
      tasks.write(t);
    }
    tasks.clear();
  }

  GCC_INLINE byte count() { return tasks.length(); }
  GCC_INLINE Task *current() { return tasks.tail(); }
  GCC_INLINE Task *next() { return tasks.back(); }
  GCC_INLINE void drop() { tasks.shift(); }
  GCC_INLINE Task *now()
  {
    // Деление происходит, только если есть память
    // Иначе деление на задачи не происходит
    Task *task = tasks.unshift();
    if (task != nullptr) task->clear();
    return task;
  }
};
