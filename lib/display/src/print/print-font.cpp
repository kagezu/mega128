#include "print-font.h"
#include <macros/context.h>

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

void PrintFont::font(const byte *font)
{
  _font = (word)font;
  _line = (1 + ((FONT_HEIGHT - 1) >> 3));
  _charSize = (FONT_WEIGHT & 0x7f) * _line;
  _offset = (word)_font + FONT_OFFSET;
  set_interline(2);
  set_interval(1);

  if (FONT_WEIGHT & 0x80)
    _offset += (FONT_COUNT + 1) * sizeof(word);
}

void PrintFont::letter(byte ch)
{
  ch -= FONT_FIRST;
  if (FONT_COUNT <= ch) ch = 0;

  byte dx = FONT_WEIGHT & 0x7f;
  byte dy = FONT_HEIGHT;
  word source;

  if (FONT_WEIGHT & 0x80) {
    word  charIndex = (word)_font + ch * sizeof(word) + FONT_OFFSET;
    source = pgm_read_word(charIndex);
    dx = (pgm_read_word(charIndex + 2) - source) / _line;
    source += _offset;
  }
  else
    source = _offset + ch * _charSize;

  if (point_x + dx > MAX_X) {
    point_y += _interline;
    point_x = 0;
  }
  if (point_y > MAX_Y - dy) point_x = point_y = 0;
  I_SAVE;
  symbol((byte *)source, point_x, point_y, dx, dy);
  I_REST;
  point_x += dx + 1;
}
