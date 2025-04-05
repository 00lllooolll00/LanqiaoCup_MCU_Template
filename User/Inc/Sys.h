#ifndef __Sys_h
#define __Sys_h

#include "main.h"
#include "Task.h"
#include "Periph.h"
#include "TIM.h"
#include "Seg.h"
#include "Key.h"
#include "iic.h"
#include "onewire.h"
#include "ds1302.h"
#include "Ultrasonic.h"
#include "Serial.h"
#include "Task_App.h"

extern uint8_t pdata Seg_Buf[8]; //数码管显示缓存
extern uint8_t pdata Seg_Point[8]; //数码管小数点显示缓存
extern uint8_t pdata Led_Buf[8]; //LED灯显示缓存

extern uint8_t pdata ucRTC[3]; //时间储存 时、分、秒
extern float temperature; //温度
extern uint8_t RxData[5]; //串口接收到的的数据
extern uint16_t Serial_Idle_Cnt; //串口空闲计时器
extern uint8_t index; //指示当前接收到数据的索引
extern uint8_t Adval1; //AD转换值
extern uint8_t Adval2; //AD转换值

#endif
