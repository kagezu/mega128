#pragma once
#include "font/font.h"

#ifndef MAX_X
#define MAX_X 0
#define MAX_Y 0
#endif

#define FONT_TAB_FACTOR     1

class PrintF {
public:
  void printf(const char *, ...);
  void print(const char *);
  void print(char *);
  void print(uint16_t);
  void print(uint32_t);
  void print(int16_t);
  void print(int32_t);
  void print_h(uint64_t);
  void print_h(uint32_t);
  void print_h(uint16_t);
  void print_h(uint8_t);
  void print(char);
  void letter(uint8_t);

  void font(const Font *);
  void at(uint16_t x, uint16_t y) { point_x = x; point_y = y; }
  void set_interline(uint8_t interline) { _interline = _font.height + interline; }
  void set_interval(uint8_t interval) { _interval = interval; }
  uint8_t get_height() { return _font.height; }
  uint8_t get_weight() { return _font.weight; }
  uint8_t get_row() { return ((uint16_t)MAX_Y + 1) / _interline; }
  uint8_t get_col() { return ((uint16_t)MAX_X + 1) / (_interval + _font.weight); }

  virtual void symbol(uint8_t *, uint16_t, uint16_t, uint8_t, uint8_t);
  // inline  virtual uint16_t min_x() = 0;
  // inline  virtual uint16_t min_y() = 0;
  // inline  virtual uint16_t max_x() = 0;
  // inline  virtual uint16_t max_y() = 0;


private:
  Font  _font = {};     // Шрифт
  uint8_t  _charSize = 0;  // Размер символа в байтах
  uint8_t  _line = 0;      // Высота символа в байтах
  uint8_t  _interline = 0; // Расстояние между строками
  uint8_t  _interval = 0;  // Расстояние между символами
  uint8_t  _tab_factor = FONT_TAB_FACTOR;
  uint16_t point_x = 0;
  uint16_t point_y = 0;

private:
  void LF() { point_y += _interline; }
  void CR() { point_x = 0; }
  void TAB() { point_x = ((point_x / ((_font.weight + _interval) << FONT_TAB_FACTOR) + 1) * (_font.weight + _interval)) << FONT_TAB_FACTOR; }
  void BS() { point_x -= (_font.weight + _interval); if (point_x > MAX_X) point_x = 0; }
  void escape() {}
};



void PrintF::font(const Font *font)
{
  memcpy_P(&_font, font, sizeof(Font));
  _line = (1 + ((_font.height - 1) >> 3));
  _charSize = _font.weight * _line;
  set_interline(2);
  set_interval(1);
}


void PrintF::letter(uint8_t ch)
{
  ch -= _font.first_char;
  if (_font.count_char <= ch) ch = 0;

  uint8_t dx = _font.weight;
  uint16_t source;

  if (_font.offset) {
    uint16_t index = _font.offset + ch * 2;
    source = pgm_read_word(index);
    dx = (pgm_read_word(index + 2) - source) / _line;
    source += _font.data;
  }
  else
    source = _font.data + ch * _charSize;


  if (point_x + dx > MAX_X) {
    point_y += _interline;
    point_x = 0;
  }
  if (point_y + _font.height > MAX_Y) { point_x = point_y = 0; }
  symbol((uint8_t *)source, point_x, point_y, dx, _font.height);
  point_x += dx + _interval;
}


void PrintF::print(char ch)
{
  switch (ch) {
    case '\f': point_x = point_y = 0; break;  // Новая страница
    case '\n': LF(); CR(); break;             // Перевод строки с возвратом
    case '\r': CR(); break;                   // Возврат каретки
    case '\b': BS(); break;                   // Шаг назад
    case '\t': TAB(); break;                  // Табуляция
    case '\v': LF(); break;                   // Вертикальная табуляция / Перевод строки
    case '\e': escape(); break;
    case '\0': point_x += _font.weight + _interval; break;
    default: if ((uint8_t)ch < 0xd0) letter(ch);
  }
}


void PrintF::printf(const char *string, ...)
{
  char ch;
  va_list args;
  va_start(args, string);

  while ((ch = pgm_read_byte(string++))) {
    switch (ch) {
      case '%': {
          char arg = '0';
          ch = pgm_read_byte(string++);
          if (ch > '/' && ch < ':') {
            arg = ch;
            ch = pgm_read_byte(string++);
          }
          switch (ch) {
            case 'c': print((char)va_arg(args, int16_t)); break;
            case 'd':
            case 'i':
              switch (arg) {
                case '0':
                case '2': print((int16_t)va_arg(args, int16_t)); break;
                case '4': print((int32_t)va_arg(args, int32_t)); break;
              } break;
            case 's': print((char *)va_arg(args, char *)); break;
            case 'S': print((const char *)va_arg(args, char *)); break;
            case 'u':
              switch (arg) {
                case '0':
                case '2': print((uint16_t)va_arg(args, uint16_t)); break;
                case '4': print((uint32_t)va_arg(args, uint32_t)); break;
              } break;
            case 'x':
              switch (arg) {
                case '0':
                case '1':  print_h((uint8_t)va_arg(args, uint16_t)); break;
                case '2':  print_h((uint16_t)va_arg(args, uint16_t)); break;
                case '4':  print_h((uint32_t)va_arg(args, uint32_t)); break;
                case '8':  print_h((uint64_t)va_arg(args, uint64_t)); break;
              } break;
            case 'p': print_h((uint16_t)va_arg(args, uint16_t)); break;
            case '%': print('%'); break;
          } break;
        }
      default: if ((uint8_t)ch < 0xd0) print(ch);
    }
  }
  va_end(args);
}
/*

void PrintF::print(char *string)
{
  while (char ch = *string++) if ((uint8_t)ch < 0xd0) print(ch);
}


void PrintF::print(const char *string)
{
  while (char ch = pgm_read_byte(string++)) if ((uint8_t)ch < 0xd0) print(ch);
}


void PrintF::print(int32_t number)
{
  if (number < 0) { print('-'); number = -number; }
  print((uint32_t)number);
}


void PrintF::print(int16_t number)
{
  if (number < 0) { print('-'); number = -number; }
  print((uint16_t)number);
}


void PrintF::print(uint32_t number)
{
  char str[11];
  char *ptr = &str[10];
  *ptr = 0;

  while (number > 9) {
    uint8_t mod;

  #ifdef ACCEL
    uint8_t tmp;
    div10_32bit(number, mod, tmp);
  #else
    mod = number % 10;
    number /= 10;
  #endif

    *--ptr = mod + '0';
  }
  *--ptr = number + '0';
  print(ptr);

}


void PrintF::print(uint16_t number)
{
  char str[7];
  char *ptr = &str[6];
  *ptr = 0;

  while (number > 9) {
    uint8_t mod;

  #ifdef ACCEL
    uint8_t tmp;
    div10_16bit(number, mod, tmp);
  #else
    mod = number % 10;
    number /= 10;
  #endif

    *--ptr = mod + '0';
  }
  *--ptr = number + '0';
  print(ptr);
}


void PrintF::print_h(uint64_t number)
{
  union { uint64_t val; struct { uint8_t a; uint8_t b; uint8_t c; uint8_t d; uint8_t e; uint8_t f; uint8_t g; uint8_t h; }; } out;
  out.val = number;
  print('#');
  print_h(out.h);
  print_h(out.g);
  print_h(out.f);
  print_h(out.e);
  print_h(out.d);
  print_h(out.c);
  print_h(out.b);
  print_h(out.a);
}


void PrintF::print_h(uint32_t number)
{
  print('#');
  print_h(to_byte(number, 3));
  print_h(to_byte(number, 2));
  print_h(to_byte(number, 1));
  print_h(to_byte(number, 0));
}


void PrintF::print_h(uint16_t number)
{
  print('#');
  print_h(to_byte(number, 1));
  print_h(to_byte(number, 0));
}


void PrintF::print_h(uint8_t number)
{
  uint8_t low = number & 0xf;
  uint8_t high = number >> 4;
  letter(high > 9 ? high + '7' : high + '0');
  letter(low > 9 ? low + '7' : low + '0');
}
*/
