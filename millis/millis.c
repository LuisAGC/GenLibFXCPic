#include <stdint.h>

#include "millis.h"

volatile uint32_t currentMillis = 0;

void updateMillis(){
    currentMillis++;
}

uint32_t millis(){
    return currentMillis;
}
