#ifndef __Serial_h
#define __Serial_h

#include "main.h"

void Uart1_Init(void);	//9600bps@12.000MHz
void Serial_SendByte(uint8_t Byte);
void Serial_SendString(uint8_t *str);

#endif
