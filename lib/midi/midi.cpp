#include "midi.h"

void MIDI::_send_msg(byte msg, byte data1, byte data2)
{
  if (msg != _run_status) {
    write(_run_status = msg);
  }
  write(data1);
  write(data2);
}

void MIDI::_send_msg(byte msg, byte data)
{
  if (msg != _run_status) {
    write(_run_status = msg);
  }
  write(data);
}

