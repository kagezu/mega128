#include "print-font.h"

void PrintFont::write(byte ch)
{
  switch (ch) {
    case '\f': point_x = point_y = 0; break;  // Новая страница
    case '\n': LF(); CR(); break;   // Перевод строки с возвратом
    case '\r': CR(); break;
    case '\b': BS(); break;
    case '\t': TAB(); break;
    case '\v': LF(); break;
    case '\e': escape(); break;
    default: if (ch < 0xd0) letter(ch);
  }
}

void PrintFont::font(const Font *font)
{
  memcpy_P(&_font, font, sizeof(Font));
  _line = (1 + ((_font.height - 1) >> 3));
  _charSize = _font.weight * _line;
  set_interline(2);
  set_interval(1);
}

void PrintFont::letter(byte ch)
{
  ch -= _font.first_char;
  if (_font.count_char <= ch) ch = 0;

  byte dx = _font.weight;
  uint16_t source;

  if (_font.offset) {
    uint16_t  index = _font.offset + ch * 2;
    source = pgm_read_uint16_t(index);
    dx = (pgm_read_uint16_t(index + 2) - source) / _line;
    source += _font.data;
  }
  else
    source = _font.data + ch * _charSize;

  if (point_x + dx > MAX_X) {
    point_y += _interline;
    point_x = 0;
  }
  if (point_y > MAX_Y - _font.height) point_x = point_y = 0;
  symbol((byte *)source, point_x, point_y, dx, _font.height);
  point_x += dx + _interval;
}
