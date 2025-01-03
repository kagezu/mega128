#include <Arduino.h>

#define MEM_FIXED 1

class MemoryBlock {
protected:
  uint16_t _link_to_ptr;
  uint16_t _start;
  uint16_t _size;

public:
  void init(uint16_t start, uint16_t size = 0)
  {
    _link_to_ptr = 0;
    _start = start;
    _size = size;
  }

public:
  void free() { _link_to_ptr = 0; }
  void use() { _link_to_ptr = MEM_FIXED; }
  void use(uint16_t link_to_ptr)
  {
    _link_to_ptr = link_to_ptr;
    *(uint16_t *)link_to_ptr = _start;
  }
  uint16_t size() { return _size; }
  uint16_t start() { return _start; }
  uint16_t link() { return _link_to_ptr; }
  void size(uint16_t new_size) { _size = new_size; }
  void start(uint16_t new_start)
  {
    _start = new_start;
    if (_link_to_ptr > MEM_FIXED)
      *(uint16_t *)_link_to_ptr = new_start;
  }

  bool is_fixed() { return _link_to_ptr == MEM_FIXED; }
  bool is_used() { return _link_to_ptr; }
  bool cmp_link(uint16_t link_to_ptr) { return link_to_ptr == _link_to_ptr; }
  bool cmp_start(uint16_t start) { return start == _start; }
};
