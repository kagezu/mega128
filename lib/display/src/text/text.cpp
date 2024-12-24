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
            case 'S': printPstr((char *)va_arg(args, char *)); break;
            case 'u':
              switch (arg) {
                case '0':
                case '1': print((uint8_t)va_arg(args, uint16_t)); break;
                case '2': print((uint16_t)va_arg(args, uint16_t)); break;
                case '4': print((uint32_t)va_arg(args, uint32_t)); break;
              } break;
            case 'x':
              switch (arg) {
                case '0':
                case '1':  printHex((uint8_t)va_arg(args, uint16_t)); break;
                case '2':  printHex((uint16_t)va_arg(args, uint16_t)); break;
                case '4':  printHex((uint32_t)va_arg(args, uint32_t)); break;
                case '8':  printHex((uint64_t)va_arg(args, uint64_t)); break;
              } break;
            case 'p': printHex((uint16_t)va_arg(args, uint16_t)); break;
            case '%': symbol('%'); break;
          } break;
        }
      case '\f': cursorX = cursorY = 0; break;  // Новая страница
      case '\n': printLF(); printCR(); break;   // Перевод строки с возвратом
      case '\r': printCR(); break;
      case '\b': printBS(); break;
      case '\t': printTAB(); break;
      case '\v': printLF(); break;
      case '\e': escape(); break;
      case '\a': bel(); break;
      default: if ((uint8_t)ch < 0xd0) symbol(ch);
    }
  }
  va_end(args);
}

void Text::font(const uint8_t *font)
{
  _font = (uint16_t)font;
  _line = (1 + ((FONT_HEIGHT - 1) >> 3));
  _charSize = (FONT_WEIGHT & 0x7f) * _line;
  _offset = (uint16_t)_font + FONT_OFFSET;
  setInterline(2);
  setInterval(1);

  if (FONT_WEIGHT & 0x80)
    _offset += (FONT_COUNT + 1) * sizeof(uint16_t);
}

void Text::symbol(uint8_t symbol)
{
  symbol -= FONT_FIRST;
  if (FONT_COUNT <= symbol) symbol = 0;

  uint8_t dx = FONT_WEIGHT & 0x7f;
  uint8_t dy = FONT_HEIGHT;
  uint16_t source;

  if (FONT_WEIGHT & 0x80) {
    uint16_t  charIndex = (uint16_t)_font + symbol * sizeof(uint16_t) + FONT_OFFSET;
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
  _display->symbol((uint8_t *)source, cursorX, cursorY, dx, dy);
  I_REST;
  cursorX += dx + 1;
}

void Text::print(const char *string)
{
  while (char ch = *string++) if ((uint8_t)ch < 0xd0) symbol(ch);
}

void Text::printPstr(const char *string)
{
  while (char ch = pgm_read_byte(string++)) if ((uint8_t)ch < 0xd0) symbol(ch);
}

void Text::print(int32_t number)
{
  if (number < 0) { symbol('-'); number = -number; }
  print((uint32_t)number);
}

void Text::print(int16_t number)
{
  if (number < 0) { symbol('-'); number = -number; }
  print((uint16_t)number);
}

void Text::print(int8_t number)
{
  if (number < 0) { symbol('-'); number = -number; }
  print((uint8_t)number);
}

void Text::print(uint32_t number)
{
  static const uint32_t mult[] PROGMEM = { 1000000000,100000000,10000000,1000000,100000,10000,1000,100,10,1 };
  char string[11];
  uint8_t i = 0, di = 0;
  string[0] = '0';
  string[1] = 0;

  for (uint8_t j = 0; j < 10; j++) {
    uint32_t m = pgm_read_dword(mult + j);
    uint8_t n = number / m;
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

void Text::print(uint16_t number)
{
  static const uint16_t mult[] PROGMEM = { 10000,1000,100,10,1 };
  char string[6];
  uint8_t i = 0, di = 0;
  string[0] = '0';
  string[1] = 0;

  for (uint8_t j = 0; j < 5; j++) {
    uint16_t m = pgm_read_word(mult + j);
    uint8_t n = number / m;
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

void Text::print(uint8_t number)
{
  static const uint8_t mult[] PROGMEM = { 100,10,1 };
  char string[4];
  uint8_t i = 0, di = 0;
  string[0] = '0';
  string[1] = 0;

  for (uint8_t j = 0; j < 3; j++) {
    uint8_t m = pgm_read_byte(mult + j);
    uint8_t n = number / m;
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

uint8_t Text::hexToChar(uint8_t number)
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

  string[17] = hexToChar(number);
  number >>= 4;
  string[16] = hexToChar(number);
  number >>= 4;
  string[15] = hexToChar(number);
  number >>= 4;
  string[14] = hexToChar(number);
  number >>= 4;
  string[13] = hexToChar(number);
  number >>= 4;
  string[12] = hexToChar(number);
  number >>= 4;
  string[11] = hexToChar(number);
  number >>= 4;
  string[10] = hexToChar(number);
  number >>= 4;
  string[9] = hexToChar(number);
  number >>= 4;
  string[8] = hexToChar(number);
  number >>= 4;
  string[7] = hexToChar(number);
  number >>= 4;
  string[6] = hexToChar(number);
  number >>= 4;
  string[5] = hexToChar(number);
  number >>= 4;
  string[4] = hexToChar(number);
  number >>= 4;
  string[3] = hexToChar(number);
  number >>= 4;
  string[2] = hexToChar(number);

  print(string);
}

void Text::printHex(uint32_t number)
{
  char string[11];
  string[0] = '0';
  string[1] = 'x';
  string[10] = 0;

  string[9] = hexToChar(number);
  number >>= 4;
  string[8] = hexToChar(number);
  number >>= 4;
  string[7] = hexToChar(number);
  number >>= 4;
  string[6] = hexToChar(number);
  number >>= 4;
  string[5] = hexToChar(number);
  number >>= 4;
  string[4] = hexToChar(number);
  number >>= 4;
  string[3] = hexToChar(number);
  number >>= 4;
  string[2] = hexToChar(number);

  print(string);
}

void Text::printHex(uint16_t number)
{
  char string[6];
  string[0] = '#';
  string[5] = 0;

  string[4] = hexToChar(number);
  number >>= 4;
  string[3] = hexToChar(number);
  number >>= 4;
  string[2] = hexToChar(number);
  number >>= 4;
  string[1] = hexToChar(number);

  print(string);
}

void Text::printHex(uint8_t number)
{
  symbol(hexToChar(number >> 4));
  symbol(hexToChar(number));
}
