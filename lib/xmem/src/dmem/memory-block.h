#include <Arduino.h>

class MemoryBlock {
protected:
  uint16_t _linkToIndex;
  uint16_t _start;
  uint16_t _length;

public:
  MemoryBlock() {}

public:
  void init(uint16_t start, uint16_t length)
  {
    _linkToIndex = 0; _start = start; _length = length;
  }

};
