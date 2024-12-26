#pragma once
#include <Arduino.h>

// макрос конкатенации макросов
#define CONCAT(x, y)      x ## y

#define DDR(x)            CONCAT(DDR, x)
#define PORT(x)           CONCAT(PORT,x)
#define PIN(x)            CONCAT(PIN, x)
