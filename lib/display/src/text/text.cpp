#include "text.h"
#include <macros/context.h>

void Text::printf(const char *string, ...)
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
            case 'c': symbol((char)va_arg(args, int)); break;
            case 'd': print((int8_t)va_arg(args, int16_t)); break;
            case 'i': print((int16_t)va_arg(args, int16_t)); break;
            case 's': print((char *)va_arg(args, char *)); break;
            case 'S': print_pstr((char *)va_arg(args, char *)); break;
            case 'u':
              switch (arg) {
                case '0':
                case '1': print((byte)va_arg(args, word)); break;
                case '2': print((word)va_arg(args, word)); break;
                case '4': print((uint32_t)va_arg(args, uint32_t)); break;
              } break;
            case 'x':
              switch (arg) {
                case '0':
                case '1':  printHex((byte)va_arg(args, word)); break;
                case '2':  printHex((word)va_arg(args, word)); break;
                case '4':  printHex((uint32_t)va_arg(args, uint32_t)); break;
                case '8':  printHex((uint64_t)va_arg(args, uint64_t)); break;
              } break;
            case 'p': printHex((word)va_arg(args, word)); break;
            case '%': symbol('%'); break;
          } break;
        }
      case '\f': cursorX = cursorY = 0; break;  // Новая страница
      case '\n': LF(); CR(); break;   // Перевод строки с возвратом
      case '\r': CR(); break;
      case '\b': BS(); break;
      case '\t': TAB(); break;
      case '\v': LF(); break;
      case '\e': escape(); break;
      case '\a': bel(); break;
      default: if ((byte)ch < 0xd0) symbol(ch);
    }
  }
  va_end(args);
}

void Text::font(const byte *font)
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

void Text::symbol(byte symbol)
{
  symbol -= FONT_FIRST;
  if (FONT_COUNT <= symbol) symbol = 0;

  byte dx = FONT_WEIGHT & 0x7f;
  byte dy = FONT_HEIGHT;
  word source;

  if (FONT_WEIGHT & 0x80) {
    word  charIndex = (word)_font + symbol * sizeof(word) + FONT_OFFSET;
    source = pgm_read_word(charIndex);
    dx = (pgm_read_word(charIndex + 2) - source) / _line;
    source += _offset;
  }
  else
    source = _offset + symbol * _charSize;

  if (cursorX + dx > MAX_X) {
    cursorY += _interline;
    cursorX = 0;
  }
  if (cursorY > MAX_Y - dy) cursorX = cursorY = 0;
  I_SAVE;
  _display->symbol((byte *)source, cursorX, cursorY, dx, dy);
  I_REST;
  cursorX += dx + 1;
}

void Text::print(const char *string)
{
  while (char ch = *string++) if ((byte)ch < 0xd0) symbol(ch);
}

void Text::print_pstr(const char *string)
{
  while (char ch = pgm_read_byte(string++)) if ((byte)ch < 0xd0) symbol(ch);
}

void Text::print(int32_t number)
{
  if (number < 0) { symbol('-'); number = -number; }
  print((uint32_t)number);
}

void Text::print(int16_t number)
{
  if (number < 0) { symbol('-'); number = -number; }
  print((word)number);
}

void Text::print(int8_t number)
{
  if (number < 0) { symbol('-'); number = -number; }
  print((byte)number);
}

void Text::print(uint32_t number)
{
  static const uint32_t mult[] PROGMEM = { 1000000000,100000000,10000000,1000000,100000,10000,1000,100,10,1 };
  char string[11];
  byte i = 0, di = 0;
  string[0] = '0';
  string[1] = 0;

  for (byte j = 0; j < 10; j++) {
    uint32_t m = pgm_read_dword(mult + j);
    byte n = number / m;
    if (n) {
      number -= m * n;
      di = 1;
    }
    string[i] = '0' + n;
    i += di;
  }
  if (di) string[i] = 0;
  print(string);
}

void Text::print(word number)
{
  static const word mult[] PROGMEM = { 10000,1000,100,10,1 };
  char string[6];
  byte i = 0, di = 0;
  string[0] = '0';
  string[1] = 0;

  for (byte j = 0; j < 5; j++) {
    word m = pgm_read_word(mult + j);
    byte n = number / m;
    if (n) {
      number -= m * n;
      di = 1;
    }
    string[i] = '0' + n;
    i += di;
  }
  if (di) string[i] = 0;
  print(string);
}

void Text::print(byte number)
{
  static const byte mult[] PROGMEM = { 100,10,1 };
  char string[4];
  byte i = 0, di = 0;
  string[0] = '0';
  string[1] = 0;

  for (byte j = 0; j < 3; j++) {
    byte m = pgm_read_byte(mult + j);
    byte n = number / m;
    if (n) {
      number -= m * n;
      di = 1;
    }
    string[i] = '0' + n;
    i += di;
  }
  if (di) string[i] = 0;
  print(string);
}

byte Text::hex_to_char(byte number)
{
  number &= 0xf;
  return number > 9 ? number + '7' : number + '0';
}

void Text::printHex(uint64_t number)
{
  char string[19];
  string[0] = '0';
  string[1] = 'x';
  string[18] = 0;

  string[17] = hex_to_char(number);
  number >>= 4;
  string[16] = hex_to_char(number);
  number >>= 4;
  string[15] = hex_to_char(number);
  number >>= 4;
  string[14] = hex_to_char(number);
  number >>= 4;
  string[13] = hex_to_char(number);
  number >>= 4;
  string[12] = hex_to_char(number);
  number >>= 4;
  string[11] = hex_to_char(number);
  number >>= 4;
  string[10] = hex_to_char(number);
  number >>= 4;
  string[9] = hex_to_char(number);
  number >>= 4;
  string[8] = hex_to_char(number);
  number >>= 4;
  string[7] = hex_to_char(number);
  number >>= 4;
  string[6] = hex_to_char(number);
  number >>= 4;
  string[5] = hex_to_char(number);
  number >>= 4;
  string[4] = hex_to_char(number);
  number >>= 4;
  string[3] = hex_to_char(number);
  number >>= 4;
  string[2] = hex_to_char(number);

  print(string);
}

void Text::printHex(uint32_t number)
{
  char string[11];
  string[0] = '0';
  string[1] = 'x';
  string[10] = 0;

  string[9] = hex_to_char(number);
  number >>= 4;
  string[8] = hex_to_char(number);
  number >>= 4;
  string[7] = hex_to_char(number);
  number >>= 4;
  string[6] = hex_to_char(number);
  number >>= 4;
  string[5] = hex_to_char(number);
  number >>= 4;
  string[4] = hex_to_char(number);
  number >>= 4;
  string[3] = hex_to_char(number);
  number >>= 4;
  string[2] = hex_to_char(number);

  print(string);
}

void Text::printHex(word number)
{
  char string[6];
  string[0] = '#';
  string[5] = 0;

  string[4] = hex_to_char(number);
  number >>= 4;
  string[3] = hex_to_char(number);
  number >>= 4;
  string[2] = hex_to_char(number);
  number >>= 4;
  string[1] = hex_to_char(number);

  print(string);
}

void Text::printHex(byte number)
{
  symbol(hex_to_char(number >> 4));
  symbol(hex_to_char(number));
}
