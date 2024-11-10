#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include "display.config.h"
#include "xmem.config.h"
#include "rgb/rgb.h"
#include "draw/draw.h"

// Команды ассемблера

#define _CLI __asm__ __volatile__("cli") // Отключает прерывания
#define _SEI __asm__ __volatile__("sei") // Включение прерываний

#endif
