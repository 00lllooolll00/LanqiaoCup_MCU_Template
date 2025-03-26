#include "LED.h"

//共阳极LED，接通低电平点亮
/**
 * @brief 对指定LED进行点亮或者熄灭的操作
 * 
 * @param pos LED的位置
 * @param enable 点亮或者熄灭
 */
void LED_Disp(uint8_t pos,bit enable)
{
    static uint8_t temp = 0x00;//为了保留之前的LED的状态 这里必须是静态变量
    static uint8_t LED_Old = 0xff;

    if(enable)
    {
        temp |= 0x01 << pos;//保留已有LED的状态之下对指定LED进行打开操作
    }
    else
    {
        temp &= ~(0x01 << pos);//保留已有LED的状态之下对指定LED进行关闭操作
    }

    if(temp != LED_Old)//消除电流声
    {
        P0 = ~temp;
        P2 = P2 & 0x1f | 0x80;
        P2 &= 0x1f;
        LED_Old = temp;
    }
}