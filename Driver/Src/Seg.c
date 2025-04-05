#include "Seg.h"
//共阳极数码管 给低电平对应位置亮
uint8_t code Seg_Table[12] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0xff, 0xbf}; //断选表
//0~9:数字0~9 10:不显示 11:-
uint8_t code Seg_Pos[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80}; //位选表

/**
 * @brief 在对应的位置的数码管显示对应的数字或者字母
 * 
 * @param pos 数码管的位置0——7
 * @param num 要显示的内容，具体参考Seg_Table数组中的数据
 * @param point 是否显示小数点
 */
void Seg_Disp(uint8_t pos, uint8_t num, bit point)
{
    //动态数码管先消影
    P0 = 0xff;
    OPEN(7);

    //先位选
    P0 = Seg_Pos[pos];
    OPEN(6);

    //后断选
    if (point)
    {
        P0 = Seg_Table[num] & 0x7f; //有小数点则给对应断选位置的最高位置0
    }
    else
    {
        P0 = Seg_Table[num];
    }
    OPEN(7);
}