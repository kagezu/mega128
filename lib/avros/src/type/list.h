#include <Arduino.h>

template <typename T, typename I>
class List {
protected:
public:
  List() {}

  // Прототипы
public:
  virtual void filter(bool callback(T *)); // Удалить несоответствующие элементы
  virtual void flat(); // Убрать дыры, обновить индексацию
  virtual bool some(bool callback(T *)); // Проверка на наличие
  virtual bool every(bool callback(T *)); // Проверка на отсутствие
  virtual bool fill(T, I, I); // Заполнение
  virtual bool sort(bool compare(T *, T *)); // Сортировка
  virtual bool splice(I start, I deleteCount = 1, T *items = nullptr, I count = 0); // Удаление/замена элементов
  virtual bool forEach(callback(T *)); // Перебор элементов

  virtual T *find(bool callback(T *)); // Поиск элемента
  virtual I findindex(bool callback(T *)); // Поиск элемента
  virtual I indexOf(T *); // Поиск элемента

  virtual void push(T);
  virtual void unshift(T);
  virtual T pop();
  virtual T shift();



};
