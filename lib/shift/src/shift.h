#include <Arduino.h>

#define SH_HQ   _BV(PC0)
#define SH_CLK  _BV(PC1)
#define SH_LD   _BV(PC2)
#define SH_SER  _BV(PC3)

#define SH_PORT PORTC
#define SH_DDR  DDRC
#define SH_PIN  PINC

#define GET_ADR(x)  ((uint8_t)&x - 0x20);


class Shift {
protected:
  uint8_t _port = _SFR_IO_ADDR(SH_PORT);
  uint8_t _ddr = _SFR_IO_ADDR(SH_DDR);
  uint8_t _pin = _SFR_IO_ADDR(SH_PIN);
  uint8_t _hq;
  uint8_t _clk;
  uint8_t _ld;
  uint8_t _ser;
};

class ShiftLoad : public Shift {
public:
  ShiftLoad()
  {
    SH_DDR |= SH_LD | SH_CLK | SH_SER;
    SH_DDR &= ~SH_HQ;
    SH_PORT &= ~SH_CLK;
    SH_PORT |= SH_LD;
  }
  // _SFR_IO_ADDR(PORT),
  // _SFR_IO_ADDR(DDR),
  // _SFR_IO_ADDR(PIN),
  // HQ, CLK, LD
  ShiftLoad(uint8_t port, uint8_t ddr, uint8_t pin, uint8_t hq, uint8_t clk, uint8_t ld)
    :
    Shift::_port(port),
    Shift::_ddr(ddr),
    Shift::_pin(pin),
    Shift::_hq(hq),
    Shift::_clk(clk),
    Shift::_ld(ld)
  {

    _SFR_IO8(_ddr) |= _ld | _clk | _ser;
    _SFR_IO8(_ddr) &= ~_hq;
    _SFR_IO8(_port) &= ~_clk;
    _SFR_IO8(_port) |= _ld;
  }

public:
  uint8_t transfer(uint8_t data)
  {
    uint8_t i = 8;
    while (i--) {
      data <<= 1;
      if (SH_PIN & SH_HQ) data++;
      PORTC |= SH_CLK;
      PORTC &= ~SH_CLK;
    }
    return data;
  }
  void write() {}
  void load()
  {
    SH_PORT &= ~SH_LD; SH_PORT |= SH_LD;
  }

  uint8_t read()
  {
    uint8_t data = 0;
    uint8_t i = 8;
    while (i--) {
      data <<= 1;
      if (SH_PIN & SH_HQ) data++;
      PORTC |= SH_CLK;
      PORTC &= ~SH_CLK;
    }
    return data;
  }

};
