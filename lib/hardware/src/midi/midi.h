#pragma once
#include "midi-types.h"

class MIDI {
public:
  void note_off(byte note, byte ch = 0, byte velocity = 0x40) { _send_msg(NoteOff | ch, note, velocity); }
  void note_on(byte note, byte ch = 0, byte velocity = 0x40) { _send_msg(NoteOn | ch, note, velocity); }
  void key_press(byte note, byte press, byte ch = 0) { _send_msg(KeyPressure | ch, note, press); }
  void ctrl_change(MIDI_Control ctrl, byte value, byte ch = 0) { _send_msg(ControlChange | ch, ctrl, value); }
  void pgm_change(byte pgm, byte ch = 0) { _send_msg(ProgramChange | ch, pgm); }
  void ch_press(byte press, byte ch = 0) { _send_msg(ChannelPressure | ch, press); }
  void pitch_bend(byte lsb = 0, byte msb = 0x20, byte ch = 0) { _send_msg(PitchBend | ch, lsb, msb); }

private:
  byte _run_status = 0;
  void _send_msg(byte, byte, byte);
  void _send_msg(byte, byte);

  virtual void write(byte) = 0;
  virtual void write(byte *, byte) = 0;
};
