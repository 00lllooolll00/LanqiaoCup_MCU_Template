#include "LED.h"

//共阳极LED，接通低电平点亮
/**
 * @brief 对指定LED进行点亮或者熄灭的操作
 * 
 * @param pos LED的位置
 * @param enable 点亮或者熄灭
 */
void LED_Disp(uint8_t pos, bit enable)
{
    static uint8_t temp = 0x00; //为了保留之前的LED的状态 这里必须是静态变量
    static uint8_t LED_Old = 0xff;

    if (enable)
    {
        temp |= 0x01 << pos; //保留已有LED的状态之下对指定LED进行打开操作
    }
    else
    {
        temp &= ~(0x01 << pos); //保留已有LED的状态之下对指定LED进行关闭操作
    }

    if (temp != LED_Old) //消除电流声
    {
        P0 = ~temp;
        P2 = P2 & 0x1f | 0x80;
        P2 &= 0x1f;
        LED_Old = temp;
    }
}

// /**
//  * @brief 基于4T测评的优化版本
//  *
//  * @param Led_Buf LED的缓存
//  */
// void Led_Disp(uint8_t *Led_Buf)
// {
//     uint8_t temp = 0;
//     static uint8_t temp_old = 0xff;

//     temp = Led_Buf[0] << 0 | Led_Buf[1] << 1 | Led_Buf[2] << 2 | Led_Buf[3] << 3 | Led_Buf[4] << 4 | Led_Buf[5] << 5 |
//            Led_Buf[6] << 6 | Led_Buf[7] << 7;

//     if (temp != temp_old)
//     {
//         P0 = ~temp;
//         P2 = P2 & 0x1f | 0x80;
//         P2 &= 0x1f;
//         temp_old = temp;
//     }
// }