#include "SysInit.h"

/**
 * @brief 关闭LED、蜂鸣器、继电器
 * 
 */
void Sys_Init(void)
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