#include <Arduino.h>
#include <type/array.h>
#include <macros/attribute.h>
#include "task.h"

#define TASK_STACK_SIZE   150
#define TASK_MAX_COUNT    8

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

  // byte count() { return tasks.length(); }
  // Task *current() { return tasks.tail(); }
  // void next() { tasks.back(); }
  // void drop() { tasks.shift(); }
  // void now() { tasks.unshift()->clear(); }

  byte count() { return tasks.length(); }
  byte heap() { return tasks.heap(); }
  Task *current() { return tasks.tail(); }
  Task *next() { return tasks.back(); }
  void drop() { if (tasks.length() > 1) tasks.shift(); }
  Task *now() //{ return tasks.unshift()->clear(); }
  {
    // Деление происходит, только если есть память
    // Иначе деление на задачи не происходит
    Task *task = tasks.unshift();
    if (task != nullptr) task->clear();
    return task;
  }
};
