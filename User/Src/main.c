/**
 * @file main.c
 * @author 九千九百九十九
 * @brief  基于西风团队的模板，加上自己写的调度器，囊括了蓝桥杯单片机的所有外设，并且为每一种外设都提供了对应的例子和注释。
 * @version 0.1
 * @date 2025-03-26
 * 
 * @copyright Copyright (c) 2025
 * 
 */
/*HEADER*/
#include "Sys.h"

/*Global Variable*/
uint32_t pdata Mysystick = 0; //系统心跳
uint8_t pdata Seg_Buf[8]; //数码管显示缓存
uint8_t pdata Seg_Point[8]; //数码管小数点显示缓存
uint8_t pdata Led_Buf[8]; //LED灯显示缓存

/*Task Creation Seg Led*/
void Task_Disp(void)
{
    uint8_t Scan = Mysystick % 8; //显示扫描变量 只在0——7周期变

    Seg_Disp(Scan, Seg_Buf[Scan], Seg_Point[Scan]); //数码管显示
    Led_Disp(Scan, Led_Buf[Scan]); //LED显示
    // Led_Disp(Led_Buf); //LED显示 4T优化版本
}

/*MAIN*/
void main(void)
{
    /*Initialize*/
    Periph_Init(); //初始化LED和外设 尽量将该模块放到第一个
    while (Read_temperature() > 80); //消除ds18b20首次转换为85的数据
    memset(Seg_Point, 0, 8); //初始化数码管小数点缓冲
    memset(Seg_Buf, 10, 8); //初始化数码管缓冲区
    memset(Led_Buf, 0, 8); //初始化LED缓冲区

    RTC_Set(ucRTC); //将需要的时间都写入RTC中
    // Timer0_Init(); //初始化TIM0
    // Uart1_Init(); //初始化串口1
    Timer2_Init(); //初始化心跳时钟源
    EA = 1;

    /*Task Add*/
    /*以下为各个模块的实例历程  取消注释即可观察对应模块的现象*/
    Task_Add(&Task_Key, 10);
    // Task_Add(&Task_NE555, 1000);
    // Task_Add(&Task_Periph, 500);
    Task_Add(&Task_RTC, 500);
    // Task_Add(&Task_Ds18b20, 750);
    // Task_Add(&Task_Ul, 200);
    // Task_Add(&Task_PCF8591, 200);
    // Task_Add(&Task_Serial, 250);

    /*Loop*/
    while (1)
    {
        Task_Start();
    }
}

/*ISR*/
/*这里使用TIM2定时器作为系统心跳源*/
void Timer2_Isr(void) interrupt 12
{
    Mysystick++;
    Serial_Idle_Cnt++;
    Task_Disp();
}

/*串口中断*/
void Uart1_Isr(void) interrupt 4
{
    if (RI) //检测串口1接收中断
    {
        RI = 0; //清除串口1接收中断请求位

        Serial_Idle_Cnt = 0; //没有接收到数据就会一直自增，接收到数据后会保持0，通过这点可以达到空闲解析的效果

        RxData[index++] = SBUF;
    }
}
