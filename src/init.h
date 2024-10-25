#include <Arduino.h>

#ifndef init_h
#define init_h

#define LCD_PORT PORTE
#define LCD_CS _BV(PB6)
#define LCD_RESET _BV(PE5)
#define LCD_RS _BV(PE4)
#define LCD_SDA _BV(PE3)
#define LCD_SCK _BV(PE2)

#define SET_BIT(port, mask) port |= mask;
#define RES_BIT(port, mask) port &= ~mask;

#endif
