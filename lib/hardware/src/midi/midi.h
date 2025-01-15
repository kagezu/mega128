#pragma once
#include "midi-types.h"
#include "VS1053/VS1053.h"

class MIDI : public VS1053 {
public:
  void note_off(byte note, byte ch = 0, byte velocity = 0x40) { _send_midi(NoteOff | ch, note, velocity); }
  void note_on(byte note, byte ch = 0, byte velocity = 0x40) { _send_midi(NoteOn | ch, note, velocity); }
  void key_press(byte note, byte press, byte ch = 0) { _send_midi(KeyPressure | ch, note, press); }
  void ctrl_change(MIDI_Control ctrl, byte value, byte ch = 0) { _send_midi(ControlChange | ch, ctrl, value); }
  void pgm_change(byte pgm, byte ch = 0) { _send_midi(ProgramChange | ch, pgm); }
  void ch_press(byte press, byte ch = 0) { _send_midi(ChannelPressure | ch, press); }
  void pitch_bend(byte lsb = 0, byte msb = 0x20, byte ch = 0) { _send_midi(PitchBend | ch, lsb, msb); }

protected:
  virtual void send_midi(byte) = 0;
  virtual void send_midi(byte, byte) = 0;
  virtual void send_midi(byte, byte, byte) = 0;

private:
  byte _run_status = 0;
  void _send_midi(byte, byte, byte);
  void _send_midi(byte, byte);
};
