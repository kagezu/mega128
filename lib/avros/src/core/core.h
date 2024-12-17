#include <Arduino.h>
#include <type/array.h>
#define TASK_MAX_COUNT      10

namespace Core {

  Array<word, byte> _adr(TASK_MAX_COUNT);

  /*
    byte async(void *callback())
    {
      word *sp = (word *)(SP + 1);
      cli();
      // char pid = _adr.add(*sp);
      sei();
      callback();
      cli();
      // _adr.get(pid);
      sei();
      return 0;// pid;
    }
  */

  // void test(void callback()) __attribute__((noinline));
  // __attribute__((noinline)) 
  void test(void callback())
  {
    cli();
    callback();
    sei();
  }
}
