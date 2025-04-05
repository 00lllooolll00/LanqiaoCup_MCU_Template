#include "Key.h"

//矩阵键盘
#if (MATRIX_Key)

/**
 * @brief 矩阵按键扫描
 * 
 * @return uint8_t 返回键值
 */
uint8_t Key_Read(void)
{
#if (NE555 && !UART) //使用了NE555且还是矩阵键盘 需要屏蔽P34
    uint8_t temp = 0; //一定要初始化为0

    P44 = 0;
    P42 = 1;
    P35 = 1; //先进行列扫描 再进行行扫描
    if (!P33) temp = 4;
    if (!P32) temp = 5;
    if (!P31) temp = 6;
    if (!P30) temp = 7;

    P44 = 1;
    P42 = 0;
    P35 = 1;
    if (!P33) temp = 8;
    if (!P32) temp = 9;
    if (!P31) temp = 10;
    if (!P30) temp = 11;

    P44 = 1;
    P42 = 1;
    P35 = 0;
    if (!P33) temp = 12;
    if (!P32) temp = 13;
    if (!P31) temp = 14;
    if (!P30) temp = 15;

    return temp;

#endif

#if (!NE555 && UART) //使用了串口 假设以定时器1作为波特率发生器
    uint8_t temp = 0; //一定要初始化为0

    TR1 = 0; //关闭波特率发生器对应的计时器

    P44 = 0;
    P42 = 1;
    P35 = 1;
    P34 = 1; //先进行列扫描 再进行行扫描
    if (!P33) temp = 4;
    if (!P32) temp = 5;
    if (!P31) temp = 6;
    if (!P30) temp = 7;

    P44 = 1;
    P42 = 0;
    P35 = 1;
    P34 = 1;
    if (!P33) temp = 8;
    if (!P32) temp = 9;
    if (!P31) temp = 10;
    if (!P30) temp = 11;

    P44 = 1;
    P42 = 1;
    P35 = 0;
    P34 = 1;
    if (!P33) temp = 12;
    if (!P32) temp = 13;
    if (!P31) temp = 14;
    if (!P30) temp = 15;

    P44 = 1;
    P42 = 1;
    P35 = 1;
    P34 = 0;
    if (!P33) temp = 16;
    if (!P32) temp = 17;
    if (!P31) temp = 18;
    if (!P30) temp = 19;

    P3 = 0xff; //恢复P3口

    TR1 = 1; //打开波特率发生器对应的计时器

    return temp;
#endif

#if (!NE555 && !UART) //串口和NE555都没用到
    uint8_t temp = 0; //一定要初始化为0

    P44 = 0;
    P42 = 1;
    P35 = 1;
    P34 = 1; //先进行列扫描 再进行行扫描
    if (!P33) temp = 4;
    if (!P32) temp = 5;
    if (!P31) temp = 6;
    if (!P30) temp = 7;

    P44 = 1;
    P42 = 0;
    P35 = 1;
    P34 = 1;
    if (!P33) temp = 8;
    if (!P32) temp = 9;
    if (!P31) temp = 10;
    if (!P30) temp = 11;

    P44 = 1;
    P42 = 1;
    P35 = 0;
    P34 = 1;
    if (!P33) temp = 12;
    if (!P32) temp = 13;
    if (!P31) temp = 14;
    if (!P30) temp = 15;

    P44 = 1;
    P42 = 1;
    P35 = 1;
    P34 = 0;
    if (!P33) temp = 16;
    if (!P32) temp = 17;
    if (!P31) temp = 18;
    if (!P30) temp = 19;

    return temp;
#endif
}

#endif

//独立按键
#if (!MATRIX_Key)

/**
 * @brief 独立按键扫描
 * 
 * @return uint8_t 返回键值
 */
uint8_t Key_Read(void)
{
    uint8_t temp = 0; //一定要初始化为0

    if (!P33) temp = 4;
    if (!P32) temp = 5;
    if (!P31) temp = 6;
    if (!P30) temp = 7;

    return temp;
}

#endif
