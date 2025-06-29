/*
 * i2c.h
 *
 * Created: 28-Mar-25 11:38:44 PM
 *  Author: admin
 */ 


#ifndef I2C_H_
#define I2C_H_

#include <avr/io.h>

#ifndef F_CPU
#define F_CPU 8000000UL  // N?u ch?a ??nh ngh?a F_CPU, m?c ??nh 8MHz
#endif

#define SCL_FREQUENCY 100000UL  // T?c ?? I2C: 100kHz

void I2C_Init();
uint8_t I2C_Transmit(uint8_t slave_address, const uint8_t *data, uint8_t length);
uint8_t I2C_Receive(uint8_t slave_address, uint8_t *buffer, uint8_t length);




#endif /* I2C_H_ */