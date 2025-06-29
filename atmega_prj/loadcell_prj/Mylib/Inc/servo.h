/*
 * servo.h
 *
 * Created: 28-Mar-25 11:19:24 PM
 *  Author: admin
 */ 


#ifndef SERVO_H_
#define SERVO_H_

#include "pwm_timer1.h"
#include <avr/io.h>

// C?u hình Timer1 ? ch? ?? Fast PWM
void Servo_Init();

// ??t góc quay c?a servo (0 - 180 ??)
void Servo1_SetAngle(uint8_t angle);
void Servo2_SetAngle(uint8_t angle);
void Servo3_SetAngle(uint8_t angle);
void Servo3_Run(void);

#endif /* SERVO_H_ */