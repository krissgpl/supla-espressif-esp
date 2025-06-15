#ifndef __I2C_MASTER_H__
#define __I2C_MASTER_H__

#include "ets_sys.h"
#include "os_type.h"
#include "osapi.h"
#include "gpio.h"

#define I2C_MASTER_SDA_GPIO 2
#define I2C_MASTER_SCL_GPIO 4

void i2c_master_gpio_init(void);
void i2c_master_init(void);
void i2c_master_start(void);
void i2c_master_stop(void);
void i2c_master_writeByte(uint8_t data);
uint8_t i2c_master_readByte(void);
bool i2c_master_checkAck(void);
void i2c_master_send_ack(void);
void i2c_master_send_nack(void);

#endif