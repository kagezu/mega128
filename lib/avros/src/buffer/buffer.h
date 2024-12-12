#include <Arduino.h>

template <typename T>
class Buffer {
protected:
  T *buffer;       // сам буфер (массив)
  byte _head;            // "голова" буфера
  byte _tail;            // "хвост" буфера
  byte _size;

public:
  Buffer(byte size) :_size(size)
  {
    buffer = (T *)malloc(size * sizeof(T));
  }

  // запись в буфер
  void write(T newVal)
  {
    // положение нового значения в буфере
    byte i = (_head + 1 >= _size) ? 0 : _head + 1;

    // если есть местечко
    if (i != _tail) {
      buffer[_head] = newVal; // пишем в буфер
      _head = i;              // двигаем голову
    }
  }
  // чтение из буфера
  T read()
  {
    if (_head == _tail) return -1;  // буфер пуст
    T thisVal = buffer[_tail];          // берём с хвоста
    if (++_tail >= _size) _tail = 0;  // хвост двигаем
    return thisVal;   // возвращаем значение
  }
  // возвращает крайнее значение без удаления из буфера
  // если буфер пуст, вернёт -1
  int peek()
  {
    return (_head != _tail) ? buffer[_tail] : -1;
  }
  // вернёт количество непрочитанных элементов
  // если буфер пуст, вернёт -1
  int amount()
  {
    return ((unsigned int)(_size + _head - _tail)) % _size;
  }
  // "очистка" буфера
  void clear()
  {
    _head = _tail = 0;
  }

};
