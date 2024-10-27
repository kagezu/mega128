#include <Arduino.h>

#ifndef init_h
#define init_h

#ifdef __AVR_ATmega128__
#define LCD_PORT PORTE
#define LCD_CS _BV(PE5)
#define LCD_RS _BV(PE4)
#define LCD_SDA _BV(PE3)
#define LCD_SCK _BV(PE2)

#define INIT_LCD_PORT                          \
  DDRE |= LCD_RS | LCD_SDA | LCD_SCK | LCD_CS; \
  PORTE |= LCD_RS | LCD_SDA | LCD_SCK | LCD_CS;
#endif

#ifdef __AVR_ATmega328P__
#define LCD_PORT PORTC
#define LCD_CS _BV(PC4)
#define LCD_RESET _BV(PC3)
#define LCD_RS _BV(PC2)
#define LCD_SDA _BV(PC1)
#define LCD_SCK _BV(PC0)

#define INIT_LCD_PORT                                      \
  DDRC |= LCD_RS | LCD_SDA | LCD_SCK | LCD_CS | LCD_RESET; \
  PORTC |= LCD_RS | LCD_SDA | LCD_SCK | LCD_CS | LCD_RESET;
#endif

#define SET_BIT(port, mask) port |= mask;
#define RES_BIT(port, mask) port &= ~mask;

#define CLI __asm__ __volatile__("cli") // Отключает прерывания
#define SEI __asm__ __volatile__("sei") // Включение прерываний

#endif
