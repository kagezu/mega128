#include <Arduino.h>

class MemoryBlock {
protected:
  uint16_t _linkToIndex = 0;
  uint16_t _start;
  uint16_t _size;

public:
  MemoryBlock(uint16_t start, uint16_t length = 0, uint16_t linkToIndex = 0)
    :_linkToIndex(linkToIndex), _start(start), _size(length)
  {}

public:
  void free() { *(uint16_t *)_linkToIndex = 0; _linkToIndex = 0; }

  void setLink(uint16_t linkToIndex) { _linkToIndex = linkToIndex; *(uint16_t *)linkToIndex = _start; }
  void setSize(uint16_t size) { _size = size; }
  void setStart(uint16_t start) { _start = start; }

  uint16_t getLink() { return _linkToIndex; }
  uint16_t getSize() { return _size; }
  uint16_t getStart() { return _start; }
};
