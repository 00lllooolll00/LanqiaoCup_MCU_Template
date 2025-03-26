#include "Delay.h"

/**
 * @brief 延时指定时间
 * 
 * @param xms 毫秒级延时
 */
void Delay(uint16_t xms)	//@12.000MHz
{
    while(xms--)
    {
        uint8_t data i, j;

        i = 12;
        j = 169;
        do
        {
            while (--j);
        } while (--i);
    }
}
