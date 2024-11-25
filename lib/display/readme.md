# Библиотека Display
---
## Обёртки
### class Display
Класс объеденующий драйвер дисплея и графическую надстройку

---
## Драйвера
### class ST7735S
Программный драйвер дисплея ST7735S.
Использует последовательный интерфейс из 4х линий:
CS,RS,SDA,SCK
### class ST7735S_SPI
Драйвер дисплея ST7735S, использующий SPI.
Последовательный интерфейс из 4х линий:
CS,RS,MOSI,SCK

---
## Графические надстройки
### class Draw
Виртуальный класс, содержащий набор графических функций
#### Список функций
##### Вспомогательные
```c++
 void fat(uint8_t f) { _fat = f; } // Установка толщены линии
  void color(RGB c) { _color = c; } // Установка цвета
  void background(RGB b) { _background = b; } // Установка фона
  void transparent(uint8_t t) { _transparent = t; }
 ```
##### Горизонтальные и вертикальные линии:
  ```c++
  void wLine(uint8_t x, uint8_t y, uint8_t x1);
  void hLine(uint8_t x, uint8_t y, uint8_t y1);
  void wLineFat(uint8_t x, uint8_t y, uint8_t x1);
  void hLineFat(uint8_t x, uint8_t y, uint8_t y1);
  ```
##### Примитивы и простые фигуры:
```c++
  inline void pixel(uint8_t x, uint8_t y) { pixel(x, y, _color); }
  inline void lineTo(uint8_t x, uint8_t y) { line(_pointX, _pointY, x, y); _pointX = x; _pointY = y; }
  void line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
  void roundRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t radius);
  void rectLine(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
  void circle(uint8_t x, uint8_t y, uint8_t  radius);
  ```
  ##### Утолщённые линии и фигуры:
  ```c++
  inline void lineToFat(uint8_t x, uint8_t y) { lineFat(_pointX, _pointY, x, y); _pointX = x; _pointY = y; }
  void lineFat(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
  void rectFat(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
  void circleFat(uint8_t x, uint8_t y, uint8_t  radius);
  ```
  ##### Полные фигуры:
  ```c++
  void circleFill(uint8_t x, uint8_t y, uint8_t  radius);
  void roundRectFill(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t  radius);
  ```
---
### class Text

Виртуальный класс, для вывода текста.

#### Список функций

В разработке.
