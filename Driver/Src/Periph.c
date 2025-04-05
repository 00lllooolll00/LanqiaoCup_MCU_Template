#include "Periph.h"

/**
 * @brief 关闭LED、蜂鸣器、继电器
 * 
 */
void Periph_Init(void)
{
    //关闭所有LED
    P0 = 0xff;
    OPEN(4);
    // P2 = P2 & 0x1f | 0x80; //先将P2的高三位清空 再将最高位置一 选择LED通道
    // P2 &= 0x1f; //P0数据取消锁存之后 重新清空P2高三位
    /*Tips：对于之后的P2的位操作不再进行赘述*/

    //关闭所有外设 eg.蜂鸣器、继电器
    P0 = 0x00;
    OPEN(5);
}

/**
 * @brief 打开/关闭指定外设
 * 
 * @param x 外设名称 RELAY MOTOR BUZZ           
 * @param enable 打开或关闭
 */
void Periph_Set(periph_t x, bit enable) //原理和LED差不多 只是这里用了枚举定义了不同的外设
{
    static uint8_t temp = 0x00;
    static uint8_t Peirph_Old = 0xff;

    if (enable)
    {
        temp |= x;
    }
    else
    {
        temp &= ~x;
    }

    if (temp != Peirph_Old)
    {
        P0 = temp;
        OPEN(5);
        Peirph_Old = temp;
    }
}

//共阳极LED，接通低电平点亮
/**
 * @brief 对指定LED进行点亮或者熄灭的操作
 * 
 * @param pos LED的位置
 * @param enable 点亮或者熄灭
 */
void Led_Disp(uint8_t pos, bit enable)
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
        OPEN(4);
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
