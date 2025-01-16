#include "midi.h"

void MIDI::_send_midi(byte msg, byte data1, byte data2)
{
  // if (msg != _run_status) {
  send_midi(_run_status = msg, data1, data2);
  // }
  // else
    // send_midi(data1, data2);
}

void MIDI::_send_midi(byte msg, byte data)
{
  // if (msg != _run_status) {
  send_midi(_run_status = msg, data);
  // }
  // send_midi(data);
}

