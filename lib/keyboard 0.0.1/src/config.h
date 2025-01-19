#pragma once
#include <core.h>

// Пины

#define CS0(x)  x ( D, 0)
#define CS1(x)  x ( D, 1)
#define QH(x)   x ( D, 3)
#define CLK(x)  x ( D, 4)
#define LD(x)   x ( D, 5)


// Частота сканирования
#define F_SCAN              500
// Максимальная скорость нажатия MIDI
#define KEY_MAX_VELOCITY    127
// 127 VELOCITY = ( 2 ^ KEY_FACTOR / F_SCAN ) секунд 
#define KEY_FACTOR          3

// Номер 1й клавиши в кодировке MIDI
#define KEY_FIRST           36
// Номер бита соответствующий 1й клавише
#define KEY_OFFSET          4
// Размер буфера
#define KEY_SIZE            8
// Количество клавиш
#define KEY_COUNT           60

