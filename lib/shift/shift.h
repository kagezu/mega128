#pragma once
#include <Arduino.h>

// Регистры _SFR_MEM_ADDR(PORTx), _SFR_MEM_ADDR(DDRx), _SFR_MEM_ADDR(PINx)
// Пины dat, clk ↑, rst (~LD / ~RESET)
class Shift {
protected:
  byte _port;
  byte _ddr;
  byte _pin;
  byte _dat;
  byte _clk;
  byte _rst;

public:
  Shift(byte port, byte ddr, byte pin, byte dat, byte clk, byte rst) :
    _port(port), _ddr(ddr), _pin(pin), _dat(_BV(dat)), _clk(_BV(clk)), _rst(_BV(rst))
  {
    _MMIO_BYTE(_ddr) |= _clk | _rst;
    _MMIO_BYTE(_ddr) &= ~_dat;
    _MMIO_BYTE(_port) &= ~_clk;
    _MMIO_BYTE(_port) |= _rst;
  }

public:
  inline void reset(boolean hold = false) { _MMIO_BYTE(_port) &= ~_rst; if (!hold)_MMIO_BYTE(_port) |= _rst; }
  inline void load() { _MMIO_BYTE(_port) &= ~_rst; _MMIO_BYTE(_port) |= _rst; }
  inline void read_bytes(byte *buffer, byte length) { while (length--) *buffer++ = read(); }
  inline void write_bytes(byte *buffer, byte length) { while (length--) write(*buffer++); }

  byte read()
  {
    byte data = 0;
    byte i = 8;
    while (i--) {
      data >>= 1;
      if (_MMIO_BYTE(_pin) & _dat) data |= 0x80;
      _MMIO_BYTE(_port) |= _clk;
      _MMIO_BYTE(_port) &= ~_clk;
    }
    return data;
  }

  void write(byte data)
  {
    byte i = 8;
    while (i--) {
      if (data & 0x80) _MMIO_BYTE(_port) |= _dat;
      else _MMIO_BYTE(_port) &= ~_dat;
      data <<= 1;
      _MMIO_BYTE(_port) |= _clk;
      _MMIO_BYTE(_port) &= ~_clk;
    }
  }
};
