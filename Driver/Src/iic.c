#include "iic.h"

#define DELAY_TIME 10

//
static void I2C_Delay(unsigned char n)
{
    do
    {
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
    } while (n--);
}

//
void I2CStart(void)
{
    sda = 1;
    scl = 1;
    I2C_Delay(DELAY_TIME);
    sda = 0;
    I2C_Delay(DELAY_TIME);
    scl = 0;
}

//
void I2CStop(void)
{
    sda = 0;
    scl = 1;
    I2C_Delay(DELAY_TIME);
    sda = 1;
    I2C_Delay(DELAY_TIME);
}

//
void I2CSendByte(unsigned char byt)
{
    unsigned char i;

    for (i = 0; i < 8; i++)
    {
        scl = 0;
        I2C_Delay(DELAY_TIME);
        if (byt & 0x80)
        {
            sda = 1;
        }
        else
        {
            sda = 0;
        }
        I2C_Delay(DELAY_TIME);
        scl = 1;
        byt <<= 1;
        I2C_Delay(DELAY_TIME);
    }

    scl = 0;
}

//
unsigned char I2CReceiveByte(void)
{
    unsigned char da;
    unsigned char i;
    for (i = 0; i < 8; i++)
    {
        scl = 1;
        I2C_Delay(DELAY_TIME);
        da <<= 1;
        if (sda) da |= 0x01;
        scl = 0;
        I2C_Delay(DELAY_TIME);
    }
    return da;
}

//
unsigned char I2CWaitAck(void)
{
    unsigned char ackbit;

    scl = 1;
    I2C_Delay(DELAY_TIME);
    ackbit = sda;
    scl = 0;
    I2C_Delay(DELAY_TIME);

    return ackbit;
}

//
void I2CSendAck(unsigned char ackbit)
{
    scl = 0;
    sda = ackbit;
    I2C_Delay(DELAY_TIME);
    scl = 1;
    I2C_Delay(DELAY_TIME);
    scl = 0;
    sda = 1;
    I2C_Delay(DELAY_TIME);
}

/**
 * @brief 从PCF8591读取AD转换的结果
 * 
 * @param addr 要读取的地址的位置
 * @return uint8_t 返回读取到的转换值
 * @note 如果题目要求读取两个AD转换的的值，需要将两个值的值的位置交换
 *       因为本次读到的值是上一次转换完成的值。
 */
uint8_t AD_Read(uint8_t addr)
{
    uint8_t temp = 0x00;

    //第一阶段 写阶段（其实是告诉从机我要从哪个地址的寄存器读取数据）
    I2CStart(); //开启通讯
    I2CSendByte(ADDR_PCF8591 & 0xfe); //对PCF8591写数据
    I2CWaitAck(); //等待从机回应
    I2CSendByte(addr); //告诉从机要读取数据的地址
    I2CWaitAck(); //等待从机回应

    //第二阶段 读数据阶段
    I2CStart(); //开启通讯
    I2CSendByte(ADDR_PCF8591 | 0x01); //读PCF8591读数据
    I2CWaitAck(); //等待从机回应
    temp = I2CReceiveByte(); //从第一阶段输入的地址读取数据，存入temp
    I2CSendAck(1); //向从机发送1，告诉从机不再发送数据
    I2CStop(); //通讯结束

    return temp;
}

/**
 * @brief 数模转换从对应引脚输出模拟量
 * 
 * @param dat 输入对应的数字量
 */
void DA_Write(uint8_t dat)
{
    I2CStart(); //开启
    I2CSendByte(ADDR_PCF8591 & 0xfe); //选中地址 0x90 写模式
    I2CWaitAck(); //等待应答
    I2CSendByte(0x43); //发送0x4x：让模拟信号输出使能
    I2CWaitAck(); //等待应答
    I2CSendByte(dat); //写入转换量
    I2CWaitAck(); //等待应答
    I2CStop(); //结束
}

/**
 * @brief 向AT24C02中被指定的地址写入对应的数据
 * 
 * @param e2prom_string 待写入的数据
 * @param addr 要写入数据的地址（最好为8的倍数）
 * @param num 页数（单页最大值为8字节）
 */
void E2prom_Write(uint8_t *e2prom_string, uint8_t addr, uint8_t num)
{
    I2CStart(); //开启i2c通讯
    I2CSendByte(ADDR_AT24C02 & 0xfe); //选中at24c02芯片 写模式 0-写模式 1-读模式
    I2CWaitAck(); //等待从机应答

    I2CSendByte(addr); //向at24c02写入数据  告知写入数据存放在at24c02芯片的地址
    I2CWaitAck(); //等待从机应答

    while (num--) //当前写入页数
    {
        I2CSendByte(*e2prom_string++); //向at24c02芯片addr地址中写入第一批数据
        I2CWaitAck(); //等待从机应答
        I2C_Delay(200); //延时
    }

    I2CStop(); //结束通讯
}

/**
 * @brief 从AT24C02的指定地址读取数据存储到数组中
 * 
 * @param e2prom_string 数据存储的数组
 * @param addr 地址（最好为8的倍数）
 * @param num 页数（单页最大值为8字节）
 */
void E2prom_Read(uint8_t *e2prom_string, uint8_t addr, uint8_t num)
{
    I2CStart(); //开启i2c通讯
    I2CSendByte(ADDR_AT24C02 & 0xfe); //选中at24c02芯片 写模式 a：1010为at24c02地址 0-写模式 1-读模式
    I2CWaitAck(); //等待从机应答

    I2CSendByte(addr); //向at24c02写入数据  告知写入数据存放在at24c02芯片的地址
    I2CWaitAck(); //等待从机应答

    I2CStart(); //开启i2c通讯
    I2CSendByte(ADDR_AT24C02 | 0x01); //选中at24c02芯片 读模式 a：1010为at24c02地址 0-写模式 1-读模式
    I2CWaitAck(); //等待从机应答

    while (num--) //当前读取页数
    {
        *e2prom_string++ = I2CReceiveByte(); //将读取到的数据存入数组e2prom_string
        if (num) I2CSendAck(0); //num不等于0的话就发送应答
        else I2CSendAck(1); //num为0代表当前循环为最后一个循环 不需要再次发送应答
    }

    I2CStop(); //结束通讯
}
