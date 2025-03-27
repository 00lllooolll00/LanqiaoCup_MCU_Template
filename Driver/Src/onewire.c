#include <onewire.h>

//
void Delay_OneWire(unsigned int t)  
{
	unsigned char i;
	while(t--){
		for(i=0;i<12;i++);
	}
}

//
void Write_DS18B20(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		DQ = 0;
		DQ = dat&0x01;
		Delay_OneWire(5);
		DQ = 1;
		dat >>= 1;
	}
	Delay_OneWire(5);
}

//
unsigned char Read_DS18B20(void)
{
	unsigned char i;
	unsigned char dat;
  
	for(i=0;i<8;i++)
	{
		DQ = 0;
		dat >>= 1;
		DQ = 1;
		if(DQ)
		{
			dat |= 0x80;
		}	    
		Delay_OneWire(5);
	}
	return dat;
}

//
bit init_ds18b20(void)
{
  	bit initflag = 0;
  	
  	DQ = 1;
  	Delay_OneWire(12);
  	DQ = 0;
  	Delay_OneWire(80);
  	DQ = 1;
  	Delay_OneWire(10); 
    initflag = DQ;     
  	Delay_OneWire(5);
  
  	return initflag;
}

/**
 * @brief 读取温度
 * 
 * @return float 温度值
 */
float Read_Temprature()
{
	uint8_t thigh,tlow;//定义高低位
	init_ds18b20();//初始化单总线
	Write_DS18B20(0xcc);//ROM指令 skip rom
	Write_DS18B20(0x44);//ROM指令 conver T
	
	init_ds18b20();//初始化单总线
	Write_DS18B20(0xcc);//ROM指令 skip rom
	Write_DS18B20(0xbe);//ROM指令 read scratchpad
	tlow = Read_DS18B20();//低位读取
	thigh = Read_DS18B20();//高位读取
	
	return((thigh << 8) | tlow) / 16.0f;//合并高低位并返回值
}