#ifndef __Periph_h
#define __Periph_h

#include "main.h"

typedef enum
{
    RELAY = 0x10,
    MOTOR = 0x20,
    BUZZ = 0x40
} periph_t;

void Periph_Init(void);
void Periph_Set(periph_t x, bit enable);
void Led_Disp(uint8_t pos, bit enable);

#endif