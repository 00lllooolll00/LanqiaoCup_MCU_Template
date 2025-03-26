#ifndef __ds1302_h
#define __ds1302_h

#include "main.h"

sbit SCK = P1 ^ 7;
sbit SDA = P2 ^ 3;
sbit RST = P1 ^ 3;

#define WRITE_PROTECT 0x8e //写保护的地址

uint8_t tobcd(uint8_t dec);
uint8_t frombcd(uint8_t bcd);
void RTC_Read(uint8_t *urRTC);
void RTC_Set(uint8_t *urRTC);

#endif
