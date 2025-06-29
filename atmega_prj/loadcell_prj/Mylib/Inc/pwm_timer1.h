/*
 * pwm_timer1.h
 *
 * Created: 28-Mar-25 11:07:15 PM
 *  Author: admin
 */ 


#ifndef PWM_TIMER1_H_
#define PWM_TIMER1_H_

#ifndef F_CPU
#define F_CPU 8000000UL // 8 MHz
#endif
#include <avr/io.h>

/* Timer 1 Mhz - pres = 8*/
void Timer1_PWM_Init();
// Hàm thi?t l?p chu kì PWM (tính b?ng micro giây)
void Timer1_SetPeriod(uint16_t period_us);
void Timer1_SetFrequency(uint16_t frequency_hz);
void Timer1_Set_Duty_OC1A(uint8_t duty);
void Timer1_Set_Pulse_OC1A(uint16_t pulse);
void Timer1_Set_Pulse_OC1B(uint16_t pulse);
void Timer0_PWM_Init(void);
void Timer0_Set_Pulse(uint16_t pulse_us);


#endif /* PWM_TIMER1_H_ */