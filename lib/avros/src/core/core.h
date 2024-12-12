#include <Arduino.h>

#define TASK_MAX_COUNT      10

class Task {
public:
  byte count = 1;
  byte pid = 0;
  byte pids[TASK_MAX_COUNT];

  byte async(void *callback())
  {
    cli();
    pid = count;
    sei();
    callback();
    cli();
    pid = 0;
    sei();
  }
};
