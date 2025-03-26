#include "Periph.h"

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