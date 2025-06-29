/*
 * pwm_timer1.c
 *
 * Created: 28-Mar-25 11:07:30 PM
 *  Author: admin
 */ 

#include "pwm_timer1.h"

/* Timer 1 Mhz - pres = 8*/
void Timer1_PWM_Init()
{
	//Timer1 mode Fast PWM v?i TOP = ICR1
	TCCR1A = (1 << WGM11) | (1 << COM1A1) |(1 << COM1B1);   // WGM11 = 1 (Fast PWM v?i TOP = ICR1) & Non-inverting mode cho OC1A
	TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS11);  // WGM12 và WGM13 = 1 & Prescaler = 8 (Timer 1 MHz)

	// C?u hình các chân PD5 (OC1A) làm output
	DDRD |= (1 << PIND5);
	DDRD |= (1 << PIND4);
}

// Hàm thi?t l?p chu kì PWM (tính b?ng micro giây)
void Timer1_SetPeriod(uint16_t period_us) {
	ICR1 = (period_us * (F_CPU / 1000000UL)) / 8 - 1; // Tính giá tr? TOP
}

void Timer1_SetFrequency(uint16_t frequency_hz) {
	ICR1 = (F_CPU / (8 * frequency_hz)) - 1; // Tính giá tr? TOP d?a trên t?n s?
}

void Timer1_Set_Duty_OC1A(uint8_t duty)
{
	// set duty cycle cho OC1A (PD5)
	OCR1A = (ICR1 * duty) / 100;  // duty tính theo %
}

void Timer1_Set_Pulse_OC1A(uint16_t pulse)
{
	OCR1A = (pulse * (F_CPU / 1000000UL)) / 8; // Chuy?n micro giây thành giá tr? PWM
}

void Timer1_Set_Pulse_OC1B(uint16_t pulse)
{
	OCR1B = (pulse * (F_CPU / 1000000UL)) / 8; // Chuy?n micro giây thành giá tr? PWM
}


void Timer0_PWM_Init()
{
	 // Fast PWM mode, non-inverting
	 TCCR0 |= (1 << WGM01) | (1 << WGM00);   // Mode 3: Fast PWM
	 TCCR0 |= (1 << COM01);                  // Clear OC0 on compare match

	 // Prescaler ch?n 64 cho phù h?p
	 // F_PWM = F_CPU / (Prescaler * 256)
	 // V?i F_CPU = 8MHz, PWM ~ 490Hz nh?ng ta ch? c?n ?? r?ng xung phù h?p servo là ???c
	 TCCR0 |= (1 << CS02) ;     // Prescaler = 256
	 DDRB |= (1 << PINB3);
}


void Timer0_Set_Pulse(uint16_t pulse_us)
{
	uint8_t ocr = (uint8_t) (pulse_us/ 32);
	if (ocr > 255) ocr = 255;
	OCR0 = ocr;
}
