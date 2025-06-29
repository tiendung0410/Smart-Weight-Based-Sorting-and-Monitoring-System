/*
 * servo.c
 *
 * Created: 28-Mar-25 11:19:42 PM
 *  Author: admin
 */ 
#include "servo.h"
#include "util/delay.h"

//pwm cycle
//1mhz -> 1 tick = 1us
//20 ms = 20000us
//500us - 2600 us
//
//arr c? ??nh = 256 tick
//8mhz / 64 -> 1 tick = 8us  -> max 2048 us
//8mhz/256 ->  1 tick 32 us -> 8192 us

#define SERVO_MIN_PULSE 500 // 1ms (góc 0 ??)
#define SERVO_MAX_PULSE 2600 // 2ms (góc 180 ??)
#define SERVO_FREQUENCY 50   // T?n s? PWM 50Hz

void Servo_Init() {
	Timer1_PWM_Init();
	Timer0_PWM_Init();
	Timer1_SetFrequency(50);
	Timer0_Set_Pulse(SERVO_MIN_PULSE);
	Timer1_Set_Pulse_OC1A(SERVO_MIN_PULSE);	
	Timer1_Set_Pulse_OC1B(SERVO_MIN_PULSE);	
}

void Servo1_SetAngle(uint8_t angle) {
	if (angle > 180) angle = 180;	
	uint16_t pulse = (uint16_t)(SERVO_MIN_PULSE + ((float)(SERVO_MAX_PULSE - SERVO_MIN_PULSE) * angle / 180.0));
	Timer1_Set_Pulse_OC1A(pulse);	
}

void Servo2_SetAngle(uint8_t angle) {
	if (angle > 180) angle = 180;
	uint16_t pulse = (uint16_t)(SERVO_MIN_PULSE + ((float)(SERVO_MAX_PULSE - SERVO_MIN_PULSE) * angle / 180.0));
	Timer1_Set_Pulse_OC1B(pulse);
}

void Servo3_SetAngle(uint8_t angle) {
	if (angle > 180) angle = 180;
	uint16_t pulse_width = (uint16_t)(SERVO_MIN_PULSE + ((float)(SERVO_MAX_PULSE - SERVO_MIN_PULSE) * angle) / 180);
	Timer0_Set_Pulse(pulse_width);
}

void Servo3_Run(void) {
	Servo3_SetAngle(180);
	_delay_ms(250);
	Servo3_SetAngle(0);
}