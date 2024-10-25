#include <Arduino.h>
#include "../../src/init.h"

// init.h
// LCD_PORT   Порт управления дисплеем
// LCD_CS     0 = Выбор дисплея / 1 = Снять выбор дисплея
// LCD_RESET  0 = Сброс дисплея (необязательно)
// LCD_RS     0 = Запись команды / 1 = Запись данных
// LCD_SDA    0/1 Данные
// LCD_SCK    Тактирование при инверсии

#define INIT_LCD_PORT                              \
  DDRE |= LCD_RS | LCD_RESET | LCD_SDA | LCD_SCK;  \
  PORTE |= LCD_RS | LCD_RESET | LCD_SDA | LCD_SCK; \
  DDRB |= LCD_CS;                                  \
  PORTB |= LCD_CS;

#define SET_CS SET_BIT(PORTB, LCD_CS)          // Снять выбор дисплея
#define SET_RESET SET_BIT(LCD_PORT, LCD_RESET) // Снять сброс дисплея
#define SET_RS SET_BIT(LCD_PORT, LCD_RS)       // Запись данных
#define SET_SDA SET_BIT(LCD_PORT, LCD_SDA)     // Данные
#define SET_SCK SET_BIT(LCD_PORT, LCD_SCK)     // Тактирование при инверсии

#define RES_CS RES_BIT(PORTB, LCD_CS)          // Выбор дисплея
#define RES_RESET RES_BIT(LCD_PORT, LCD_RESET) // Сброс дисплея
#define RES_RS RES_BIT(LCD_PORT, LCD_RS)       // Запись команды
#define RES_SDA RES_BIT(LCD_PORT, LCD_SDA)     // Данные
#define RES_SCK RES_BIT(LCD_PORT, LCD_SCK)     // Тактирование при инверсии

#define TICK_TSK RES_SCK SET_SCK

class ST7735S
{
public:
  // byte pixelFormat;

public:
  ST7735S(byte format = 0x03);
  void command(byte data);
  void setBlock(byte x1, byte y1, byte x2, byte y2);
  void data_12(word data);
  void data_8(byte data);
  void data_0();
  void data_rgb(byte r, byte g, byte b);
  void pixel(byte x, byte y, word color);
  void rect(byte x1, byte y1, byte x2, byte y2, word color);

  inline void clear(word color) { rect(0, 0, 127, 159, color); };

  void test(byte d);
};
