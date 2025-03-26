#ifndef __onewire_h
#define __onewire_h

#include "main.h"

sbit DQ = P1 ^ 4;

float Read_Temprature();

#endif