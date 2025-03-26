#include "ds1302.h"

//
void Write_Ds1302(unsigned  char temp) 
{
	unsigned char i;
	for (i=0;i<8;i++)     	
	{ 
		SCK = 0;
		SDA = temp&0x01;
		temp>>=1; 
		SCK=1;
	}
}   

//
void Write_Ds1302_Byte( unsigned char address,unsigned char dat )     
{
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1; 	_nop_();  
 	Write_Ds1302(address);	
 	Write_Ds1302(dat);		
 	RST=0; 
}

//
unsigned char Read_Ds1302_Byte ( unsigned char address )
{
 	unsigned char i,temp=0x00;
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1;	_nop_();
 	Write_Ds1302(address);
 	for (i=0;i<8;i++) 	
 	{		
		SCK=0;
		temp>>=1;	
 		if(SDA)
 		temp|=0x80;	
 		SCK=1;
	} 
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
	SCK=1;	_nop_();
	SDA=0;	_nop_();
	SDA=1;	_nop_();
	return (temp);			
}

/**
 * @brief 将十进制转换为bcd码
 * 
 * @param dec 待转换的十进制数
 * @return uint8_t 返回一个bcd码的数
 */
uint8_t tobcd(uint8_t dec)
{
	uint8_t bcd;
	bcd=dec/10*16+dec%10;
	return bcd;
}

/**
 * @brief 将bcd码转化为十进制
 * 
 * @param bcd 待转换的bcd码
 * @return uint8_t 转换后的十进制
 */
uint8_t frombcd(uint8_t bcd)
{
	uint8_t dec;
	dec=bcd/16*10+bcd%16;
	return dec;
}

/**
 * @brief 从芯片中读取数据，存储到传入的数组中
 * 
 * @param urRTC 存储数据的数组
 */
void RTC_Read(uint8_t* urRTC)
{
	uint8_t i;
	for(i=0;i<3;i++)//循环3位 分别把芯片中的时分秒读入数组ucRTC
	{
		urRTC[i]=frombcd(Read_Ds1302_Byte(0x85-2*i));//我们需要的是十进制 但是芯片存储的是bcd码 所以需要转译之后再存入数组
	}
}

/**
 * @brief 将传入数组中的数据写入芯片中
 * 
 * @param urRTC 写入数据的数组
 */
void RTC_Set(uint8_t* urRTC)
{
	uint8_t i;
	Write_Ds1302_Byte(WRITE_PROTECT,0x00);//关闭写入保护
	for(i=0;i<3;i++)//循环3位 分别把ucRTC数组中的时分秒写入芯片
	{
		Write_Ds1302_Byte(0x84-2*i,tobcd(urRTC[i]));//数组中的为十进制 但是芯片中需要的是bcd 所以存入芯片之前要先转变成bcd
	}
	Write_Ds1302_Byte(WRITE_PROTECT,0x80);//打开写入保护
}
