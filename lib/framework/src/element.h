// #include "element.h"
#pragma once
#include "core.h"

class  Element {
public:
  byte type;
  Element *parent;

public:
  Element *create_element(byte);
  void draw();
};


