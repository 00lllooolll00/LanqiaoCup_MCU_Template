#include "Periph.h"

/**
 * @brief 关闭LED、蜂鸣器、继电器
 * 
 */
void Periph_Init(void)
{
    //关闭所有LED
    P0 = 0xff;
    P2 = P2 & 0x1f | 0x80;//先将P2的高三位清空 再将最高位置一 选择LED通道
    P2 &= 0x1f;//P0数据取消锁存之后 重新清空P2高三位
    /*Tips：对于之后的P2的位操作不再进行赘述*/

    //关闭所有外设 eg.蜂鸣器、继电器
    P0 = 0x00;
    P2 = P2 & 0x1f | 0xa0;
    P2 &= 0x1f;
}

/**
 * @brief 打开/关闭指定外设
 * 
 * @param x 外设名称 RELAY MOTOR BUZZ           
 * @param enable 打开或关闭
 */
void Periph_Set(periph_t x,bit enable)//原理和LED差不多 只是这里用了枚举定义了不同的外设
{
    static uint8_t temp = 0x00;
    static uint8_t Peirph_Old = 0xff;

    if(enable)
    {
        temp |= x;
    }
    else
    {
        temp &= ~x;
    }

    if(temp != Peirph_Old)
    {
        P0 = temp;
        P2 = P2 & 0x1f | 0xa0;
        P2 &= 0x1f;
        Peirph_Old = temp;
    }
}