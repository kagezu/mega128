// LCD_PORT   Порт управления дисплеем
// LCD_CS     0 = Выбор дисплея / 1 = Снять выбор дисплея
// LCD_RS     0 = Запись команды / 1 = Запись данных
// LCD_SDA    0/1 Данные
// LCD_SCK    Тактирование при инверсии
// INIT_LCD_PORT Инициализация DDRx

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
