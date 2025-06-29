/*
 * gpio.h
 *
 * Created: 05-Apr-25 12:05:38 AM
 *  Author: admin
 */ 


#ifndef GPIO_H_
#define GPIO_H_

#include <avr/io.h>

// LED ??nh danh
#define LED_GREEN 0
#define LED_YELLOW 1
#define LED_RED 2

// Macros bit t??ng ?ng v?i PA0, PA1, PA2
#define LED_GREEN_PIN PINA1
#define LED_YELLOW_PIN PINA2
#define LED_RED_PIN PINA3

void LED_Init(void);

void LED_On(uint8_t led);

void LED_Off(uint8_t led);

void LED_Toggle(uint8_t led);

void Button_Init(void);

void IR_Sensors_Init(void);

uint8_t IR_Sensor_Read(uint8_t sensor);

void Alarm_Init(void);

void Alarm(void);

#endif /* GPIO_H_ */