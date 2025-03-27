#include "Serial.h"

/**
 * @brief 初始化串口 设置波特率为9600
 * @note 这是基于TIM1来做的串口
 */
void Uart1_Init(void)	//9600bps@12.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR &= 0xBF;		//定时器时钟12T模式
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0xE6;			//设置定时初始值
	TH1 = 0xFF;			//设置定时初始值
	ET1 = 0;			//禁止定时器中断
	TR1 = 1;			//定时器1开始计时
	ES = 1;				//使能串口1中断
    EA = 1;//这句话是定时器使能，要自己加上！！！！！！！！！！
}

/**
 * @brief 通过串口发送一字节
 * 
 * @param Byte 要发送的数据
 */
void Serial_SendByte(uint8_t Byte)
{
    SBUF = Byte;//将数据放到SBUF寄存器中 串口会自己发送数据
    while(!TI);//等待TI置位置数 TI发送完成之后会自动置1
    TI = 0;//清除TI标志位
}

/**
 * @brief 通过串口发送一个字符串
 * 
 * @param str 
 */
void Serial_SendString(uint8_t *str)
{
    while(*str != '\0')//字符串总是以'\0'结尾
    {
        Serial_SendByte(*str ++);
    }
}

/**
 * @brief printf重定向
 * 
 * @param ch 
 * @return int 
 */
char putchar(char c)
{
    Serial_SendByte(c);
    return c;
}

/*串口中断
void Uart1_Isr(void) interrupt 4
{
	if (RI)				//检测串口1接收中断
	{
		RI = 0;			//清除串口1接收中断请求位
	}
}
*/