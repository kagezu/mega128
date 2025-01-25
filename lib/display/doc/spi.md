# ST7735S_SPI

Типовая схема подключения при использовании SPI.
Выводы настраиваются в файле [display.config.h](../src/display.config.h)

|ST7735S|ATmega 128|ATMega328P|
|----|----|----|
| CS  | PE0 | PC5     |
| RS  | PE1 | PC4     |
| SDA | PB2 | PB3     |
| SCK | PB1 | PB5     |
| RST |RESET| RESET   |
