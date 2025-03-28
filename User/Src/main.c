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
#include "main.h"
#include "Task.h"
#include "Periph.h"
#include "TIM.h"
#include "LED.h"
#include "Seg.h"
#include "Key.h"
#include "Delay.h"
#include "iic.h"
#include "onewire.h"
#include "ds1302.h"
#include "Ultrasonic.h"
#include "Serial.h"

/*Global Variable*/
uint32_t xdata Mysystick = 0;//系统心跳
uint8_t pdata Seg_Buf[8];//数码管显示缓存
uint8_t pdata Seg_Point[8];//数码管小数点显示缓存
uint8_t pdata ucLed[8];//LED灯显示缓存
uint8_t pdata ucRTC[3] = {23,59,50};//时间储存 时、分、秒
float temperature;//温度
uint8_t RxData[5];//串口接收到的的数据
uint16_t Serial_Idle_Cnt;//串口空闲计时器
uint8_t index;//指示当前接收到数据的索引
uint8_t Adval1;//AD转换值
uint8_t Adval2;//AD转换值

/*Function Prototype*/
/**
 * @brief 滤波器 使连续变化的数据更加平滑
 * 
 * @param dat 输入连续变化的待测数据
 * @param index 滤波系数 0-1 越大滤波力度越大
 * @return uint16_t 返回滤波后的值
 * 
 * @note 如果需要对float类型的数据进行滤波 请重写本函数 如下所示
 */
uint16_t Filter_int(uint16_t dat, float index)
{
    static uint16_t LastData_int = 0;
    uint16_t temp;
    if(!LastData_int)LastData_int = dat;
    temp = LastData_int * index + dat * (1 - index);
    LastData_int = temp;
    return temp;
}

/**
 * @brief 对浮点类型、连续变化的数据滤波
 * 
 * @param dat 输入连续变化的待测数据
 * @param index 滤波系数 0-1 越大滤波力度越大
 * @return float 返回滤波后的值
 */
float Filter_float(float dat, float index)
{
    static float LastData_float = 0;
    float temp;
    if(!LastData_float)LastData_float = dat;
    temp = LastData_float * index + dat * (1 - index);
    LastData_float = temp;
    return temp;
}


/*Task1 Creation Key*/
void Task_Key(void)
{
    static uint8_t Key_Val;
    static uint8_t Key_Down;
    static uint8_t Key_Up;
    static uint8_t Key_Old = 0;//该值初始给0 避免警告

    Key_Val = Key_Read();
    Key_Down = Key_Val & (Key_Val ^ Key_Old);
    Key_Up = ~Key_Val & (Key_Val ^ Key_Old);
    Key_Old = Key_Val;

    /*Test*/
    if(Key_Down == 4)
    {
        static bit Test_Flag = 0;
        Test_Flag ^= 1;
        memset(ucLed,Test_Flag,8);
        memset(Seg_Point,Test_Flag,8);
    }

}

/*Task2 Creation Seg Led*/
void Task_Disp(void)
{
    uint8_t Scan = Mysystick % 8;//显示扫描变量 只在0——7周期变

    Seg_Disp(Scan,Seg_Buf[Scan],Seg_Point[Scan]);//数码管显示
    LED_Disp(Scan,ucLed[Scan]);//LED显示
}

/*Task3 Creation NE555*/
void Task_NE555(void)
{
    static uint16_t Freq = 0;//NE555频率

    //显示数据
    //为避免第一次测量时没数据，先初始化为0，先显示第一次再开始测量
	Seg_Buf[0] = Freq / 10000 % 10;
	Seg_Buf[1] = Freq / 1000 % 10;
	Seg_Buf[2] = Freq / 100 % 10;
	Seg_Buf[3] = Freq / 10 % 10;
	Seg_Buf[4] = Freq % 10;
    
    TR0 = 0;//关闭TIM0计数
    ET1 = 0;//关闭TIM1中断允许
    Freq = TH0 << 8 | TL0;//获取频率
    TH0 = TL0 = 0;//手动清空计数值
    ET1 = 1;//打开TIM1中断允许
    TR0 = 1;//开启TIM0计数

}

/*Task4 Creation Buzz Motor Relay*/
void Task_Periph(void)
{
    static bit Flag_Buzz = 0;
    static bit Flag_Relay = 1;

    Periph_Set(BUZZ,Flag_Buzz);
    Periph_Set(RELAY,Flag_Relay);

    //500ms翻转一次继电器和蜂鸣器的电平
    Flag_Buzz ^= 1;
    Flag_Relay ^= 1;
}

/*Task5 Creation DS1302*/
void Task_RTC(void)
{
    uint8_t i;
	RTC_Read(ucRTC);
	Seg_Buf[2] = Seg_Buf[5] = 11;
    for(i = 0;i < 3; i++)
    {
        Seg_Buf[i * 3] = ucRTC[i] / 10;
        Seg_Buf[i * 3 + 1] = ucRTC[i] % 10;   
    }
}

/*Task6 Creation DS18B20*/
void Task_Ds18b20(void)
{
    float temperature_show;//展示的温度值
    temperature = Read_temperature();//获取温度
    temperature_show = Filter_float(temperature, 0.3);//滤波

    //数据显示
    Seg_Point[1] = 1;//显示小数点
    Seg_Buf[0] = (uint8_t)(temperature_show * 10) / 100 % 10;
    Seg_Buf[1] = (uint8_t)(temperature_show * 10) / 10 % 10;
    Seg_Buf[2] = (uint8_t)(temperature_show * 10) % 10;
}

/*Task7 Creation Ultrasonic*/
void Task_Ul(void)
{
    static uint8_t distance;
    static uint8_t distance_Show;
    distance = Distance_Get();//获取距离
    distance_Show = (uint8_t)Filter_int(distance, 0.2);//滤波

    //显示距离
	Seg_Buf[0] = distance_Show / 100 % 10;
	Seg_Buf[1] = distance_Show / 10 % 10;
	Seg_Buf[2] = distance_Show % 10;
}

/*Task8 Creation Serial*/
void Task_Serial(void)
{
    static uint8_t psc = 0;//分频器 每1s发送一次    
    
    /*模拟串口发送数据 这里是1s打印一个数据*/
    if(++psc == 4)
    {
        psc = 0;
        printf("systime:%d\r\n", (uint16_t)(Mysystick / 1000));
    }

    if(Serial_Idle_Cnt > 300)//300ms没接受到数据就会解析一次 这里可以自行调整
    {
        /*串口解析操作*/
        if(RxData[0] == '1')
        {
            static bit i = 0;
            index = 0;
            RxData[0] = 0;
            memset(ucLed, i, 8);
            i ^= 1;
        }
    }
}

/*Task9 Creation PCF8951*/
void Task_PCF8951(void)
{
    uint16_t Adval1_Show;//滤波后的展示值
    uint16_t Adval2_Show;//滤波后的展示值

    Adval1 = AD_Read(0x41);//电位器
    Adval2 = AD_Read(0x43);//光敏

    Adval1_Show = Filter_int(Adval1, 0.2);//滤波
    Adval2_Show = Filter_int(Adval2, 0.2);
    /*
        tips:   如果只读一个AD值 0x41为光敏 0x43为电位器
                但是由于本次读取到的数据是上一次转换的结果。
                所以连续读数据的话需要将两个通道反转一下。
    */
    DA_Write(Adval1 / 51);//输出对应电压

    Seg_Buf[0] = Adval1_Show / 100 % 10;
	Seg_Buf[1] = Adval1_Show / 10 % 10;
	Seg_Buf[2] = Adval1_Show % 10;

    Seg_Buf[5] = Adval2_Show / 100 % 10;
	Seg_Buf[6] = Adval2_Show / 10 % 10;
	Seg_Buf[7] = Adval2_Show % 10;
}

/*MAIN*/
void main(void)
{
    /*Initialize*/
    Periph_Init();//初始化LED和外设 尽量将该模块放到第一个
    while(Read_temperature() > 80);//消除ds18b20首次转换为85的数据
    memset(Seg_Point,0,8);//初始化数码管小数点缓冲
    memset(Seg_Buf,10,8);//初始化数码管缓冲区
    memset(ucLed,0,8);//初始化LED缓冲区

    RTC_Set(ucRTC);//将需要的时间都写入RTC中
    // Timer0_Init();//初始化TIM0
    // Uart1_Init();//初始化串口1
    Timer2_Init();//初始化心跳时钟源

    /*Task Add*/
    /*以下为各个模块的实例历程  取消注释即可观察对应模块的现象*/
    Task_Add(&Task_Key, 10);
    // Task_Add(&Task_NE555, 1000);
    // Task_Add(&Task_Periph, 500);
    // Task_Add(&Task_RTC, 500);
    Task_Add(&Task_Ds18b20, 750);
    // Task_Add(&Task_Ul, 200);
    // Task_Add(&Task_Serial, 250);
    // Task_Add(&Task_PCF8951, 200);

    /*Loop*/
    while(1)
    {
        Task_Start();
    }
}

/*ISR*/
/*这里使用TIM2定时器作为系统心跳源*/
void Timer2_Isr(void) interrupt 12
{
    Mysystick ++;
    Serial_Idle_Cnt ++;
    Task_Disp();
}

/*串口中断*/
void Uart1_Isr(void) interrupt 4
{
	if (RI)				//检测串口1接收中断
	{
		RI = 0;			//清除串口1接收中断请求位

        Serial_Idle_Cnt = 0;//没有接收到数据就会一直自增，接收到数据后会保持0，通过这点可以达到空闲解析的效果

		RxData[index ++] = SBUF;
	}
}
