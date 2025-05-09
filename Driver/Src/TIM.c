#include "TIM.h"

/**
 * @brief 初始化TIM0，并开启计时器
 * 
 */
void Timer0_Init(void)		//1毫秒@12.000MHz
{
	#if(NE555)
	/*	
	Tips:
		定时器模式:16位（不可自动重装）
		定时器时钟:12T
		定时时长:随便选之后给TH0 TL0手动给0
		不要打开中断
		并且要先初始化定时器0在初始化定时器1
	*/
	AUXR &= 0x7F;			//定时器时钟12T模式
	TMOD &= 0xF0;			//设置定时器模式
	TMOD |= 0x05;//一定要手动设置该位置，是为了设置TIM0为16位不可重装的计数模式
	TL0 = 0x00;				//设置定时初始值
	TH0 = 0x00;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	#endif
	
	#if (!NE555)
	AUXR &= 0x7F;			//定时器时钟12T模式
	TMOD &= 0xF0;			//设置定时器模式
	TMOD |= 0x01;			//设置定时器模式
	TL0 = 0x18;				//设置定时初始值
	TH0 = 0xFC;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	#endif
}

/*定时器0中断
void Timer0_Isr(void) interrupt 1
{
}
*/

/**
 * @brief 初始化TIM1，开启计时 
 * @note 如果涉及到NE555测量 需要且只能用TIM0用作计数器，所以这个时候心跳时钟只能由TIM1来提供
 * 
 */
void Timer1_Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0xBF;			//定时器时钟12T模式
	TMOD &= 0x0F;			//设置定时器模式
	TL1 = 0x18;				//设置定时初始值
	TH1 = 0xFC;				//设置定时初始值
	TF1 = 0;				//清除TF1标志
	TR1 = 1;				//定时器1开始计时
	ET1 = 1;				//使能定时器1中断
}

/*定时器1中断
void Timer1_Isr(void) interrupt 3
{
}
*/

/**
 * @brief  初始化TIM2，开启计时 
 * @note 强烈建议用TIM2作为系统的时基心跳源！！！
 * 
 */
void Timer2_Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x04;			//定时器时钟1T模式
	T2L = 0x20;				//设置定时初始值
	T2H = 0xD1;				//设置定时初始值
	AUXR |= 0x10;			//定时器2开始计时
	IE2 |= 0x04;			//使能定时器2中断
}

/*定时器2中断
void Timer2_Isr(void) interrupt 12
{
}
*/