#include "element.h"

#define ELEMENT_MAX_COUNT 20

static Array<Element, byte> elements(ELEMENT_MAX_COUNT);
static void draw_element(Element *el, void *parent) { if (el->parent == parent) el->draw(); }

Element *Element::create_element(byte type)
{
  Element *elem = elements.push();
  elem->type = type;
  elem->parent = this;
  return elem;
}

void Element::draw()
{
  elements.each(draw_element, this);
}
