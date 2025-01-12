#pragma once

// Сообщить компилятору, что структура должна быть максимально упакована,
// никаких лишних байтов на выравнивание добавляться не должно.
// Необходимо для экономии памяти.
#define GCC_PACKED __attribute__((packed))

// Сообщить компилятору, что функция не возвращает управление вызывающему коду.
#define GCC_NORETURN __attribute__((noreturn))

// Сообщить компилятору, что функция без параметров, пролога,
// эпилога и возвращаемого значения.
#define GCC_NAKED __attribute__((naked))

// Сообщить компилятору, что функция часто вызывается.
#define GCC_HOT __attribute__((hot))

// Сообщить компилятору, что функция редко вызывается.
#define GCC_COLD __attribute__((cold))

// Сообщить компилятору, что функция где-то используется.
// Иногда функция, которая используется только из ассемблерной вставки,
// компилятором может быть удалена за ненадобностью, потому что компилятор
// не понимает, что функция все же нужна. Этот макрос заставляет его
// сохранить функцию в собранном бинарнике, чтобы ее потом нашел линковщик.
#define GCC_USED __attribute__((used))

// Сообщить компилятору, что функция представляет из себя задачу
// операционной системы. Эта функция не сохраняет значения некоторых
// регистров, но при этом безопасна для прерываний.
#define GCC_OS_TASK __attribute__((OS_task))

// Сообщить компилятору, что функция представляет из себя главную функцию
// операционной системы. Эта функция не сохраняет значения некоторых
// регистров и небезопасна для прерываний.
#define GCC_OS_MAIN __attribute__((OS_main))

// Отметить, что конкретные параметры функции не могут иметь
// значение 0. Может применяться только к параметрам-указателям.
// Может улучшить оптимизацию кода и позволяет добавить
// некоторые проверки на этапе компиляции.
#define GCC_NONNULL(...) __attribute__((nonnull(__VA_ARGS__)))

// Использование атрибута weak для объекта при его публичном определении
// делает это определение "слабым", которое можно переопределить.
#define GCC_WEAK __attribute__((weak));

// .initX, где Х - номер секции от 0 до 9. Это секция для кода.
// Сразу после сброса выполняется код, помещенный в секцию .init0,
// потом постепенно номер секции увеличивается, и функция main выполняется в секции 9.
// Параметры стека задаются в секции 2, а до этого стек еще не определен.
#define GCC_INIT(	X ) static void __attribute__ ((naked, used, section (".init" #X ))) __init ## X (void)

// В порядке уменьшения своего номера секции выполняются ПОСЛЕ ВЫХОДА ИЗ main,
// то есть сначала код из секции .fini9, потом меньше и меньше,
// и в 0-й реализуется запрет прерываний и m1: rjmp m1.
#define GCC_FINI(	X ) static void __attribute__ ((naked, used, section (".fini" #X ))) __fini ## X (void)

// Все переменные, помещенные в эту секцию, не будут инициализированы "по умолчанию".
#define GCC_NO_INIT   __attribute__ ((section (".noinit")))

// Принудительно сделать inline
#define GCC_INLINE    __attribute__ ((always_inline))

// Попытаться избежать inline вставок
#define GCC_NO_INLINE __attribute__ ((noinline))

// Функция не используют никаких значений, кроме своих аргументов, и не имеет эффекта, кроме возвращаемого значения.
// Такая функция может быть объектом исключения общих подвыражений и оптимизации циклов аналогично арифметической операции.
// Такую функцию следует объявить с атрибутом const.
// Функцию безопасно вызывать меньшее количество раз, чем сказано в программе. 
#define GCC_CONST     __attribute__ ((const))
