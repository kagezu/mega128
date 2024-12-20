#include <Arduino.h>

#define MEM_UNLOCK  0
#define MEM_LOCK    1
#define MEM_FIXED   2

class MemoryBlock {
protected:
  word _linkToIndex;
  word _start;
  word _size;
  volatile byte _status;

public:
  MemoryBlock(word start, word length = 0, word linkToIndex = 0)
    :_linkToIndex(linkToIndex), _start(start), _size(length), _status(MEM_UNLOCK)
  {}

public:
  inline void free() { *(word *)_linkToIndex = 0; _linkToIndex = 0; }

  inline void setLink(word linkToIndex) { _linkToIndex = linkToIndex; *(word *)linkToIndex = _start; }
  inline void setSize(word size) { _size = size; }
  inline void setStart(word start) { _start = start; }

  inline void lock() { _status |= MEM_LOCK; }
  inline void unlock() { _status &= ~MEM_LOCK; }
  inline void fixed() { _status |= MEM_FIXED; }

  inline word getLink() { return _linkToIndex; }
  inline word getSize() { return _size; }
  inline word getStart() { return _start; }
  inline byte getStatus() { return _status; }
};
