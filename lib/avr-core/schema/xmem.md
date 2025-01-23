# e**X**tended **MEM**ory

Схема подключения внешней SDRAM.
Подключения выводов A16 и выше заносятся в файл [xmem.config.h](../src/xmem.config.h)

|ATmega 128|K6X1008C2D|HC573|
|:---:|:---:|:---:|
| PA0 | -   | D0 |
| PA1 | -   | D1 |
| PA2 | -   | D2 |
| PA3 | -   | D3 |
| PA4 | -   | D4 |
| PA5 | -   | D5 |
| PA6 | -   | D6 |
| PA7 | -   | D7 |
| -   | A0  | Q0 |
| -   | A1  | Q1 |
| -   | A2  | Q2 |
| -   | A3  | Q3 |
| -   | A4  | Q4 |
| -   | A5  | Q5 |
| -   | A6  | Q6 |
| -   | A7  | Q7 |
| PC0 | A8  | -  |
| PC1 | A9  | -  |
| PC2 | A10 | -  |
| PC3 | A11 | -  |
| PC4 | A12 | -  |
| PC5 | A13 | -  |
| PC6 | A14 | -  |
| PC7 | A15 | -  |
| PG0 | ~WE | -  |
| PG1 | ~OE | -  |
| PG2 | -   |~LE |
| Gnd |~CS1 |~OE |
| Vcc | CS2 | -  |
| **PE6** | **A16** | -  |
