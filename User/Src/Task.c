/**
 * @file Task.c
 * @author 九千九百九十九
 * @brief 基于时间片轮询的一种裸机任务调度方案
 * @version 0.1
 * @date 2025-03-26
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "Task.h"

uint8_t Task_Num = 0;//任务数量
task_t xdata Task_Schedule[MAXTASKS];//任务列表 由于过大 存储到xdata区

/**
 * @brief 创建一任务
 * 
 * @param pfunc 该任务的函数指针
 * @param differ_time 任务执行间隔 ms
 */
void Task_Add(task_func pfunc,uint16_t differ_time)
{
    if(Task_Num <= MAXTASKS)
    {
        Task_Schedule[Task_Num].LastWakeUp = Mysystick;
        Task_Schedule[Task_Num].Interval = differ_time;
        Task_Schedule[Task_Num].Function = pfunc;
        Task_Num ++;//任务数量自增
    }
}

/**
 * @brief 任务开始运行
 * 
 */
void Task_Start(void)
{
    uint8_t i;
    for(i = 0;i < Task_Num;i ++)
    {
        if(Mysystick - Task_Schedule[i].LastWakeUp >= Task_Schedule[i].Interval)
        {
            Task_Schedule[i].LastWakeUp = Mysystick;//更新上次调用任务的时间
            Task_Schedule[i].Function();
        }
    }
}