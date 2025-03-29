#ifndef __iic_h
#define __iic_h

#include "main.h"

sbit sda = P2 ^ 1; //在使用sbit的时候一定要带上^
sbit scl = P2 ^ 0;

#define ADDR_PCF8591 0x90 //PCF8591的地址
#define ADDR_AT24C02 0xa0 //AT24C02的地址

uint8_t AD_Read(uint8_t addr);
void DA_Write(uint8_t dat);

void E2prom_Write(uint8_t *e2prom_string, uint8_t addr, uint8_t num);
void E2prom_Read(uint8_t *e2prom_string, uint8_t addr, uint8_t num);

#endif