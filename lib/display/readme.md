# Библиотека Display
---
## Обёртки
### <span style="color:blue">class</span> **Display**
Класс для вывода на дисплей

### <span style="color:blue">class</span> **Screen**
Класс для вывода в память

---
## Драйвера
### <span style="color:blue">class</span> **ST7735S**
Программный драйвер дисплея ST7735S.
Использует последовательный интерфейс из 4х линий:
CS,RS,SDA,SCK

### <span style="color:blue">class</span> **ST7735S_SPI**
Драйвер дисплея ST7735S, использующий SPI.
Последовательный интерфейс из 4х линий:
CS,RS,MOSI,SCK

---

## Графические надстройки
### <span style="color:blue">class</span> **Draw**

Kласс, содержащий набор графических функций
+ Линии
+ Прямоугольные области
+ круги
+ скруглённые прямоугольники

### <span style="color:blue">class</span> **Text**

Класс, для вывода текста.
+ Вывод текста нескольких шрифтов
+ Вывод 10х и 16х целых чисел.

---

## Цветовые модели

### <span style="color:blue">class</span> **RGB**

#### <span style="color:green">file</span> **rgb12.h**
12ти битный цветовой формат: Red 4 bit x Green 4 bit x Blue 4 bit
#### <span style="color:green">file</span> **rgb16.h**
16ти битный цветовой формат: Red 5 bit x Green 6 bit x Blue 5 bit
#### <span style="color:green">file</span> **rgb18.h**
18ти битный цветовой формат: Red 6 bit x Green 6 bit x Blue 6 bit

