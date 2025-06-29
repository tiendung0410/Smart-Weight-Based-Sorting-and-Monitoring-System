/*
 * gpio.c
 *
 * Created: 05-Apr-25 12:05:49 AM
 *  Author: admin
 */ 
#include "gpio.h"

#ifndef F_CPU
#define F_CPU 8000000UL // 8 MHz
#endif

#include "util/delay.h"

void LED_Init(void) {
	// C?u h�nh PA0, PA1, PA2 l� output
	DDRA |= (1 << LED_GREEN_PIN) | (1 << LED_YELLOW_PIN) | (1 << LED_RED_PIN);
	// T?t t?t c? ?�n ban ??u
	PORTA &= ~((1 << LED_GREEN_PIN) | (1 << LED_YELLOW_PIN) | (1 << LED_RED_PIN));
}

void LED_On(uint8_t led) {
	switch (led) {
		case LED_GREEN: PORTA |= (1 << LED_GREEN_PIN); break;
		case LED_YELLOW: PORTA |= (1 << LED_YELLOW_PIN); break;
		case LED_RED: PORTA |= (1 << LED_RED_PIN); break;
		default: break;
	}
}

void LED_Off(uint8_t led) {
	switch (led) {
		case LED_GREEN: PORTA &= ~(1 << LED_GREEN_PIN); break;
		case LED_YELLOW: PORTA &= ~(1 << LED_YELLOW_PIN); break;
		case LED_RED: PORTA &= ~(1 << LED_RED_PIN); break;
		default: break;
	}
}

void LED_Toggle(uint8_t led) {
	switch (led) {
		case LED_GREEN: PORTA ^= (1 << LED_GREEN_PIN); break;
		case LED_YELLOW: PORTA ^= (1 << LED_YELLOW_PIN); break;
		case LED_RED: PORTA ^= (1 << LED_RED_PIN); break;
		default: break;
	}
}

void Button_Init(void)
{
	// C?u h�nh PD2 (INT0) l� input
	DDRD &= ~(1 << PD2);

	// K�ch ho?t ?i?n tr? k�o l�n (pull-up) n?u c?n
	PORTD |= (1 << PD2);

	// C?u h�nh ?i?u ki?n sinh ng?t l� Falling Edge (xung xu?ng m?c th?p)
	MCUCR |= (1 << ISC01);    // ISC01 = 1
	MCUCR &= ~(1 << ISC00);   // ISC00 = 0

	// Cho ph�p ng?t ngo�i INT0
	GICR |= (1 << INT0);

}

void IR_Sensors_Init(void)
{
    // Cấu hình PC2 và PC3 là input
    DDRC &= ~((1 << PINC2) | (1 << PINC3));
    // Kích hoạt điện trở kéo lên (pull-up) cho PC2 và PC3
    PORTC |= (1 << PINC2) | (1 << PINC3);
}

uint8_t IR_Sensor_Read(uint8_t sensor)
{
    // sensor = 1 cho PC2, sensor = 2 cho PC3
    if (sensor == 1)
        return (PINC & (1 << PINC2)) ? 1 : 0; // 1: phát hiện vật cản, 0: không có vật cản
    else if (sensor == 2)
        return (PINC & (1 << PINC3)) ? 1 : 0;
    else
        return 0;
}

void Alarm_Init(void)
{
	// C?u h�nh PA0, PA1, PA2 l� output
	DDRA |= (1 << PINA4);
	// T?t t?t c? ?�n ban ??u
	PORTA &= ~(1 << PINA4);
}

void Alarm(void)
{
	PORTA |= (1 << PINA4);
	_delay_ms(50);
	PORTA &= ~(1 << PINA4);
}