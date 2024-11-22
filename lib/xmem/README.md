## class XPage
Библиотека для **ATmega128 + eXternal MEMory (XMEM)**

### Архитектура
|Адрес|Аппаратно|
|----|----|
| ***0x0000 - 0x00FF*** | **Регистры**|
| ***0x0100 - 0x10FF*** | **Внутрення SDRAM**|
| ***0x1100 - 0xFFFF*** | **Внешняя XMEM**|

#### Значение сегментов по умолчанию:
`.stack` = ***0x10FF*** (Значение записываемое в регистр `SP` сразу после `RESET`)

`.data` = ***0x0100*** (С этого адреса компилятор располагает все глобальные переменные, константы и объекты)

Для изменения этих значений можно добавить ключи в [*platformio.ini*](../../platformio.ini)
```ini
build_flags =
    -Wl,--defsym=__stack=0x03FF ;Новое значение стека
    -Wl,-Tdata,0x800400 ;Новое значение для данных (0x800000 указывает на SDRAM)
```

### Использование
#### Инициализация:
Предварительно в файле [*config.h*](../xmem/src/config.h) необходимо указать пины для адресации памяти, если используется более 64kB

#### Пример для 128kB XMEM:
```c++
XPage page0(0x000,XMEM_60K,0x1000);
XPage page1(0x100,XMEM_60K,0x1100);
XPage page2(0x000,XMEM_4K,0x1100);
XPage page3(0x100,XMEM_4K,0x1100);
XPage page4(0x010,XMEM_0K,0x1100);
XPage page5(0x110,XMEM_0K,0x1100);
```
| Выбор страницы | Физические адреса | Логические адреса | Комментарий |
|-|-|-|-|
| page0.use(); | ***0x01100 - 0x0FFFF*** | ***0x1000 - 0xFFFF*** | 0xF000 байт, включающее в себя SRAM + XMEM |
| page1.use(); | ***0x11100 - 0x1FFFF*** | ***0x1100 - 0xFFFF*** | 0xEF00 байт, только XMEM |
| page2.use(); | ***0x00000 - 0x00FFF*** | ***0x1100 - 0x20FF*** | 0x1000 байт, остаток page0 скрытый за внутренней памятью |
| page3.use(); | ***0x10000 - 0x10FFF*** | ***0x1100 - 0x20FF*** | 0x1000 байт, остаток page1 скрытый за внутренней памятью |
| page4.use(); | ***0x01000 - 0x010FF*** | ***0x1100 - 0x11FF*** | 0x0100 байт, не использованный ранее остаток [1] |
| page5.use(); | ***0x11000 - 0x110FF*** | ***0x1100 - 0x11FF*** | 0x0100 байт, всё! теперь исчерпаны все 128kB |

1. Последние 2е страницы по 256 байт, можно было включить в page\[2-3], указав маску `XMEM_8K`, но тогда становиться доступна и часть памяти из page\[0-1]

#### Выделение памяти и использование ссылок:
```c++
typedef uint16_t(&DIM)[100][100];
DIM dim = (DIM)*page1.malloc(sizeof(DIM));
```
Далее `dim` можно использовать как обычный массив, не забывая на какой странице он находиться:
```c++
  page1.use();
  dim[40][50];
```