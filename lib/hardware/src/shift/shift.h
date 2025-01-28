#pragma once
#include <Arduino.h>

// Регистры _SFR_MEM_ADDR(PORTx), _SFR_MEM_ADDR(DDRx), _SFR_MEM_ADDR(PINx)
// Пины dat, clk ↑, rst (~LD / ~RESET)
class Shift {
protected:
  uint8_t _port;
  uint8_t _ddr;
  uint8_t _pin;
  uint8_t _dat;
  uint8_t _clk;
  uint8_t _rst;

public:
  Shift(uint8_t port, uint8_t ddr, uint8_t pin, uint8_t dat, uint8_t clk, uint8_t rst) :
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
  inline void read_bytes(uint8_t *buffer, uint8_t length) { while (length--) *buffer++ = read(); }
  inline void write_bytes(uint8_t *buffer, uint8_t length) { while (length--) write(*buffer++); }

  uint8_t read()
  {
    uint8_t data = 0;
    uint8_t i = 8;
    while (i--) {
      data >>= 1;
      if (_MMIO_BYTE(_pin) & _dat) data |= 0x80;
      _MMIO_BYTE(_port) |= _clk;
      _MMIO_BYTE(_port) &= ~_clk;
    }
    return data;
  }

  void write(uint8_t data)
  {
    uint8_t i = 8;
    while (i--) {
      if (data & 0x80) _MMIO_BYTE(_port) |= _dat;
      else _MMIO_BYTE(_port) &= ~_dat;
      data <<= 1;
      _MMIO_BYTE(_port) |= _clk;
      _MMIO_BYTE(_port) &= ~_clk;
    }
  }
};
