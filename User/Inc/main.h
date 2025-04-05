#ifndef __main_h
#define __main_h

#include <STC15F2K60S2.H>
#include <intrins.h>
#include <string.h>
#include <stdio.h>

#define MATRIX_Key 1 //如果需要矩阵键盘，将该值设置为1。如果为独立按键，则把该值设置为0
#define NE555      1 //如果使用到了NE555的测量 则把该值设定为1 并且保证Timer0在Timer1之前初始化
#define UART       0 //如果使用到了串口，将该宏定义设定为1 否则为0

typedef unsigned char uint8_t;
typedef unsigned int uint16_t; //根据不同处理器的架构不同，在51中int型一般占用2个字节，在32中int型占用4个字节
typedef unsigned long uint32_t;

typedef char int8_t;
typedef int int16_t;
typedef long int32_t;

#define OPEN(X)                                                                                                        \
    do                                                                                                                 \
    {                                                                                                                  \
        P2 = P2 & 0x1f | (X << 5);                                                                                     \
        P2 &= 0x1f;                                                                                                    \
    } while (0);

extern uint32_t pdata Mysystick;

#endif
