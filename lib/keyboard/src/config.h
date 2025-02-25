#pragma once
#include <core.h>

// Пины

#define CS0(x)  x ( D, 0)
#define CS1(x)  x ( D, 1)
#define QH(x)   x ( D, 3)
#define CLK(x)  x ( D, 4)
#define LD(x)   x ( D, 5)


// Частота сканирования
#define F_SCAN              400

// Номер 1й клавиши в кодировке MIDI
#define KEY_FIRST           36
// Номер бита соответствующий 1й клавише
#define KEY_OFFSET          4
// Размер буфера
#define KEY_SIZE            8
// Количество клавиш
#define KEY_COUNT           60
// Размер буфера нажатий
#define  KEY_BUFFER_SIZE    5

#define  KEY_MASK_PREFIX    0xC0
#define  KEY_ON_PREFIX      0x00
#define  KEY_OFF_PREFIX     0x40

struct Key {
  uint8_t num;
  uint8_t value;
};

extern Buffer<Key, uint8_t> KeyBuffer;

// Дисплей

// Выбор типа дисплея
#define _ST7735_
#define LCD_SOFT            // Программно
#define RGB_FORMAT RGB_12   // 4x4x4 bit

// #define FLIP_X
#define FLIP_Y
#define EX_X_Y

#include "display.h"
