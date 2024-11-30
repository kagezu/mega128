#include "AY/AY.h"

class NumPad {
private:
  AY *_psg;
  byte _key = 0;

public:
  NumPad(AY *psg) :_psg(psg) {}

public:
  byte getKey()
  {
    byte key = _psg->getKey();
    if (key == _key) key = 0;
    else _key = key;
    return key;
  }

  byte getSymbol()
  {
    switch (_key & 0x1f) {
      case 0x01: return '1';
      case 0x02: return '4';
      case 0x03: return '7';
      case 0x04: return 8; // BS
      case 0x09: return '2';
      case 0x0A: return '5';
      case 0x0B: return '8';
      case 0x0C: return '0';
      case 0x11: return '3';
      case 0x12: return '6';
      case 0x13: return '9';
      case 0x14: return 9; // TAB
      case 0x19: return 11;// VT
      case 0x1A: return 10;// LF
      case 0x1B: return 27;// ESC
      case 0x1C: return 13;// CR
    }
    switch (_key & 0xe0) {
      case 0x00: return 0;
      case 0x20: return 14;
      case 0x40: return 15;
      case 0x60: return '#'; // BS
      case 0x80: return '*';
      case 0xE0: return '@';
    }
    return _key;
  }
};
