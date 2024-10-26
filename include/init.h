#include <Arduino.h>

#ifndef init_h
#define init_h

#define LCD_PORT PORTE
#define LCD_CS _BV(PE5)
#define LCD_RS _BV(PE4)
#define LCD_SDA _BV(PE3)
#define LCD_SCK _BV(PE2)

#define INIT_LCD_PORT                          \
  DDRE |= LCD_RS | LCD_SDA | LCD_SCK | LCD_CS; \
  PORTE |= LCD_RS | LCD_SDA | LCD_SCK | LCD_CS;

#define SET_BIT(port, mask) port |= mask;
#define RES_BIT(port, mask) port &= ~mask;

#define CLI __asm__ __volatile__("cli") // Отключает прерывания
#define SEI __asm__ __volatile__("sei") // Включение прерываний

#endif
