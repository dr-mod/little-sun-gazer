#include "ds3231.h"

//regaddrs,seconds,minutes,hours,weekdays,days,months,years
uint8_t  buf[]={0x00,0x50,0x59,0x17,0x04,0x05,0x03,0x21};
char  *week[]={"SUN","Mon","Tues","Wed","Thur","Fri","Sat"};
int ds3231SetTime()
{
    uint8_t buf[2];
	//set second
	buf[0]=0x00;
	buf[1]=0x50;
	i2c_write_blocking(I2C_PORT,addrs,buf,2,false);
	//set minute
	buf[0]=0x01;
	buf[1]=0x59;
	i2c_write_blocking(I2C_PORT,addrs,buf,2,false);
	//set hour
	buf[0]=0x02;
	buf[1]=0x23;
	i2c_write_blocking(I2C_PORT,addrs,buf,2,false);
	//set weekday
	buf[0]=0x03;
	buf[1]=0x04;
	i2c_write_blocking(I2C_PORT,addrs,buf,2,false);
	//set day
	buf[0]=0x04;
	buf[1]=0x05;
	i2c_write_blocking(I2C_PORT,addrs,buf,2,false);
	//set month
	buf[0]=0x05;
	buf[1]=0x03;
	i2c_write_blocking(I2C_PORT,addrs,buf,2,false);
	//set year
	buf[0]=0x06;
	buf[1]=0x21;
	i2c_write_blocking(I2C_PORT,addrs,buf,2,false);
	return 0;
}

void ds3231ReadTime() 
{   
	uint8_t val = 0x00;  
	i2c_write_blocking(I2C_PORT,addrs,&val,1,true);
	i2c_read_blocking(I2C_PORT,addrs,buf,7,false);
} 

