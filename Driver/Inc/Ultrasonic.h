#ifndef __Ultrasonic_h
#define __Ultrasonic_h

#include "main.h"

sbit Tx = P1 ^ 0;
sbit Rx = P1 ^ 1;

uint8_t Distance_Get(void);

#endif