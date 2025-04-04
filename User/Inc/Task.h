#ifndef __Task_h
#define __Task_h

#include "main.h"

#define MAXTASKS 5 //最大任务数目

typedef void (*task_func)(void); //定义任务执行函数

typedef struct
{
    uint32_t LastWakeUp; //上一次执行本任务时间
    uint16_t Interval; //需要的时间间隔
    task_func Function; //任务执行的函数
} task_t;

void Task_Add(task_func pfunc, uint16_t differ_time);
void Task_Start(void);

#endif
