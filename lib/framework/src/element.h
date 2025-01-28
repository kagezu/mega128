// #include "element.h"
#pragma once
#include "core.h"

class  Element {
public:
  uint8_t type;
  Element *parent;

public:
  Element *create_element(uint8_t);
  void draw();
};


