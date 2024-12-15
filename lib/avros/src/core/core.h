#include <Arduino.h>
#include <type/array.h>

#define TASK_MAX_COUNT      10

class Task {
protected:
  Array<void *, char> _adr;

public:
  Task() :_adr(TASK_MAX_COUNT) {}

  byte async(void *callback())
  {
    cli();
    char pid = _adr.add((void *)*(uint16_t *)SP);
    sei();
    callback();
    cli();
    _adr.get(pid);
    sei();
  }
};
