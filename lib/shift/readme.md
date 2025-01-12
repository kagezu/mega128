# Библиотека Shift

Для работы со сдвиговыми регистрами:
**MC54/74HC164** - Параллельный вывод
**MC54/74HC165** - Параллельный ввод

---

### <span style="color:blue">class</span> **Shift**

+ **Shift**( PORTx, DDRx, PINx, dat, clk, rst/ld ) В конструктор передать адреса портов в/в и номера пинов
+ **reset**( boolean **)** - false = удерживать сброс **MC54/74HC164**
+ **load()** - загрузить через параллельный вход **MC54/74HC165**
+ **readbytes**( byte *buffer, byte length ) - считать через последовательный вход
+ **writebytes**( byte *buffer, byte length **)** - записать через последовательный выход

#### <span style="color:green">Пример использования</span>

Считывание 8ми регистров 74HC165 в 64х битную переменную:

```c+
#include "shift.h"

Shift keyboard(
  _SFR_MEM_ADDR(PORTC),
  _SFR_MEM_ADDR(DDRC),
  _SFR_MEM_ADDR(PINC),
  PC0,
  PC1,
  PC2
);
uint64_t keys;

int main()
{
  keyboard.load();
  keyboard.readbytes((byte *)&keys, sizeof(keys));
  // ...
}
```
