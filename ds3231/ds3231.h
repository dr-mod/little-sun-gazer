#ifndef __DS3231_H
#define __DS3231_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define addrs 0x68

#define I2C_PORT	i2c0
#define I2C_SCL		20	
#define I2C_SDA		21

int ds3231SetTime();
void ds3231ReadTime();

#endif

