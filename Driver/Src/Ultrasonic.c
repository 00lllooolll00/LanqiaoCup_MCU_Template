#include "Ultrasonic.h"

/**
 * @brief 12us延时 为超声波发生提供方波
 * 
 */
void Delay12us(void)	//@12.000MHz
{
	unsigned char data i;

	_nop_();
	_nop_();
	i = 33;
	while (--i);
}

/**
 * @brief 发送8次40Khz的方波
 * 
 */
void Ul_Wave(void)
{
    uint8_t i;
    for(i = 0; i < 8; i ++)
    {
        Tx = 1;
        Delay12us();
        Tx = 0;
        Delay12us();
    }
}

/**
 * @brief 读取距离
 * 
 * 
 * @return 返回距离
 * @note 使用PCA来测距
 */
uint8_t Distance_Get(void)
{
    uint16_t time;
    CH = CL = 0;//PCA的高低计数器设置为0
    CMOD = 0x00;//设置PCA模式为定时模式

    Ul_Wave();//发送方波

    CR = 1;//PCA开始计时
    while(Rx && !CF);//如果Rx非0说明接收到数据，跳出循环。或者CF置1，表示PCA溢出，证明测量已经溢出了，也要跳出循环。
    CR = 0;//PCA停止计时

    if(!CF)//如果CF没有置1，说明没有溢出，正常计算数据然后返回
    {
        time = CH << 8 | CL;//先计算得到时间，由时间计算得到下面的距离
        return (0.017 * time) + 3;
    }
    else//如果CF置1了，代表溢出，清除CF的标志位并且返回0
    {
        CF = 0;
        return 0;
    }

}