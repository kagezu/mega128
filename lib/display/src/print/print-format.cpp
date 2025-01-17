#include "print-format.h"
#include <macros/accel.h>

template class PrintF<byte>;
template class PrintF<uint16_t>;

template<typename I>
void PrintF<I>::font(const Font *font)
{
  memcpy_P(&_font, font, sizeof(Font));
  _line = (1 + ((_font.height - 1) >> 3));
  _charSize = _font.weight * _line;
  set_interline(2);
  set_interval(1);
}

template<typename I>
void PrintF<I>::letter(byte ch)
{
  ch -= _font.first_char;
  if (_font.count_char <= ch) ch = 0;

  byte dx = _font.weight;
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
  symbol((byte *)source, point_x, point_y, dx, _font.height);
  point_x += dx + _interval;
}

template<typename I>
void PrintF<I>::print(char ch)
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
    default: if ((byte)ch < 0xd0) letter(ch);
  }
}

template<typename I>
void PrintF<I>::printf(const __FlashStringHelper *p, ...)
{
  PGM_P string = reinterpret_cast<PGM_P>(p);
  char ch;
  va_list args;
  va_start(args, p);

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
            case 'S': print((__FlashStringHelper *)va_arg(args, char *)); break;
            case 'u':
              switch (arg) {
                case '0':
                case '2': print((uint16_t)va_arg(args, uint16_t)); break;
                case '4': print((uint32_t)va_arg(args, uint32_t)); break;
              } break;
            case 'x':
              switch (arg) {
                case '0':
                case '1':  print_h((byte)va_arg(args, uint16_t)); break;
                case '2':  print_h((uint16_t)va_arg(args, uint16_t)); break;
                case '4':  print_h((uint32_t)va_arg(args, uint32_t)); break;
                case '8':  print_h((uint64_t)va_arg(args, uint64_t)); break;
              } break;
            case 'p': print_h((uint16_t)va_arg(args, uint16_t)); break;
            case '%': print('%'); break;
          } break;
        }
      default: if ((byte)ch < 0xd0) print(ch);
    }
  }
  va_end(args);
}

template<typename I>
void PrintF<I>::print(char *string)
{
  while (char ch = *string++) if ((byte)ch < 0xd0) print(ch);
}

template<typename I>
void PrintF<I>::print(const __FlashStringHelper *string)
{
  PGM_P ptr = reinterpret_cast<PGM_P>(string);
  while (char ch = pgm_read_byte(ptr++)) if ((byte)ch < 0xd0) print(ch);
}

template<typename I>
void PrintF<I>::print(const char *string)
{
  while (char ch = pgm_read_byte(string++)) if ((byte)ch < 0xd0) print(ch);
}

template<typename I>
void PrintF<I>::print(int32_t number)
{
  if (number < 0) { print('-'); number = -number; }
  print((uint32_t)number);
}

template<typename I>
void PrintF<I>::print(int16_t number)
{
  if (number < 0) { print('-'); number = -number; }
  print((uint16_t)number);
}

template<typename I>
void PrintF<I>::print(uint32_t number)
{
  char str[11];
  char *ptr = &str[10];
  *ptr = 0;

  while (number > 9) {
    byte mod;

  #ifdef ACCEL
    byte tmp;
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

template<typename I>
void PrintF<I>::print(uint16_t number)
{
  char str[7];
  char *ptr = &str[6];
  *ptr = 0;

  while (number > 9) {
    byte mod;

  #ifdef ACCEL
    byte tmp;
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

template<typename I>
void PrintF<I>::print_h(uint64_t number)
{
  union { uint64_t val; struct { byte a; byte b; byte c; byte d; byte e; byte f; byte g; byte h; }; } out;
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

template<typename I>
void PrintF<I>::print_h(uint32_t number)
{
  print('#');
  print_h(to_byte(number, 3));
  print_h(to_byte(number, 2));
  print_h(to_byte(number, 1));
  print_h(to_byte(number, 0));
}

template<typename I>
void PrintF<I>::print_h(uint16_t number)
{
  print('#');
  print_h(to_byte(number, 1));
  print_h(to_byte(number, 0));
}

template<typename I>
void PrintF<I>::print_h(byte number)
{
  byte low = number & 0xf;
  byte high = number >> 4;
  letter(high > 9 ? high + '7' : high + '0');
  letter(low > 9 ? low + '7' : low + '0');
}
