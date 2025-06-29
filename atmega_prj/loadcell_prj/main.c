/*
 * loadcell_prj.c
 *
 * Created: 28-Mar-25 2:42:29 PM
 * Author : admin
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include "uart.h"
#include "hx711.h"
#include "servo.h"
#include "i2c.h" 
#include "uart.h"
#include "gpio.h"
#include "util/delay.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdint.h"
#include "string.h"
#include "stdlib.h"
#include <avr/interrupt.h>

#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1 //U2X = 0
#define LOADCELL_SCALE 5.9661016949152 // scale
#define LOADCELL_OFFSET 8397621     // Offset
#define MIN_WEIGHT 10
#define SERVO_WORKING_ANGLE 45
#define SLAVE_ADDRESS 0x28

uint16_t threshold_1 = 1000;
uint16_t threshold_2 = 2000;
uint16_t light_preset = 1000;
uint16_t medium_preset = 10000;
uint16_t heavy_preset = 10000;
uint16_t light_incont = 0;
uint16_t medium_incont = 0;
uint16_t heavy_incont = 0;
uint8_t total_quantity = 0;
volatile uint8_t power = 0;
uint8_t full = 0;
uint16_t weight = 0;
char snd_buf[256];
volatile uint8_t json_parsed_flag = 0;


uint16_t extract_uint16(const char* json, const char* key) {
	char* pos = strstr(json, key);
	if (pos) {
		pos = strchr(pos, ':');
		if (pos) {
			return (uint16_t)atoi(pos + 1);
		}
	}
	return 0;
}

void parse_json(const char* json) {
	threshold_1	   = extract_uint16(json, "\"Threshold_1\"");;
	threshold_2    = extract_uint16(json, "\"Threshold_2\"");
	light_preset   = extract_uint16(json, "\"Light_Preset\"");
	medium_preset  = extract_uint16(json, "\"Medium_Preset\"");
	heavy_preset   = extract_uint16(json, "\"Heavy_Preset\"");
	power          = (uint8_t)extract_uint16(json, "\"Power\""); // true/false s? l� 1/0
}

uint8_t calculate_checksum(uint8_t *data, uint16_t length) {
	uint8_t checksum = 0;
	for (int i = 0; i < length; i++) {
		checksum ^= data[i];
	}
	return checksum;
}


void send_data_to_qt(void)
{
	// T?o chu?i JSON
	memset(snd_buf, 0, sizeof(snd_buf));
	snprintf(snd_buf, sizeof(snd_buf),
	"{\"Threshold_1\":%d,\"Threshold_2\":%d,\"Last_Weight\":%d,\"Light_Preset\":%d,\"Medium_Preset\":%d,\"Heavy_Preset\":%d,\"Light_Cont\":%d,\"Medium_Cont\":%d,\"Heavy_Cont\":%d,\"Total_Quantity\":%d,\"Power\":%d,\"Full\":%d}",
	threshold_1, threshold_2, weight, light_preset, medium_preset, heavy_preset, light_incont, medium_incont, heavy_incont, total_quantity, power, full);
	USART_Send_String(snd_buf);
	uint16_t data_len = strlen(snd_buf);
	
	// T�nh checksum cho to�n b? d? li?u
	uint8_t checksum = calculate_checksum((uint8_t *)snd_buf, data_len);
	
	// Th�m checksum v�o cu?i
	snd_buf[data_len] = checksum;
	data_len++; // T?ng ?? d�i th�m 1 cho byte checksum
	
	// G?i d? li?u theo t?ng ph?n n?u d? li?u qu� l?n
	const uint8_t MAX_CHUNK_SIZE = 150; // Gi?i h?n an to�n
	uint16_t sent = 0;
	
	while (sent < data_len) {
		uint8_t chunk_size = (data_len - sent > MAX_CHUNK_SIZE) ? MAX_CHUNK_SIZE : (data_len - sent);
		
		// G?i ph?n hi?n t?i
		I2C_Transmit(SLAVE_ADDRESS, (uint8_t *)(snd_buf + sent), chunk_size);
		
		sent += chunk_size;
		_delay_ms(10); // Ch? m?t ch�t gi?a c�c l?n g?i
	}
	//
	//// G?i th�ng b�o qua USART ?? debug
	//USART_Send_String("Data sent in ");
	//char tmp[10];
	//sprintf(tmp, "%d", (sent + MAX_CHUNK_SIZE - 1) / MAX_CHUNK_SIZE);
	//USART_Send_String(tmp);
	//USART_Send_String(" chunks\r\n");
}

ISR(USART_RXC_vect)
{
	uart_rx_handle();
	if(rx_flag)
	{
		parse_json((const char*)rx_buf);

		// C?p nh?t LED tr?ng th�i
		if(!power){
			LED_Off(LED_GREEN);
			LED_On(LED_YELLOW);
		}
		else{
			LED_On(LED_GREEN);
			LED_Off(LED_YELLOW);
		}

		memset(rx_buf, 0, sizeof(rx_buf));
		rx_flag = 0;
		json_parsed_flag = 1; // ??t c? cho main x? l�
	}
}

ISR(INT0_vect)
{
	_delay_ms(10);   

	if (!(PIND & (1 << PIND2)))  
	{
		if(power){
			power = 0;
			LED_Off(LED_GREEN);	
			LED_On(LED_YELLOW);
			USART_Send_String("power off\n");	
		}
		else{
			power = 1;
			LED_On(LED_GREEN);
			LED_Off(LED_YELLOW);
			USART_Send_String("power on\n");
		}	
	}
	
}


//void send_data_to_qt(void)
//{
	//memset(snd_buf, 0, sizeof(snd_buf));
	//snprintf(snd_buf, sizeof(snd_buf),
	//"{\"threshold_1\":%d,\"threshold_2\":%d,\"last_weight\":%d,\"light_preset\":%d,\"medium_preset\":%d,\"heavy_preset\":%d,\"light_cont\":%d,\"medium_cont\":%d,\"heavy_cont\":%d,\"total_quantity\":%d,\"power\":%d,\"full\":%d}",
	//threshold_1, threshold_2, weight, light_preset, medium_preset, heavy_preset, light_incont, medium_incont, heavy_incont, total_quantity, power, full);
	//
	//uint16_t data_len = strlen(snd_buf);
	//
	//// g?i chu?i g?c qua usart tr??c
	//USART_Send_String(snd_buf);
	//
	//// t�nh to�n v� th�m checksum v�o cu?i
	//uint8_t checksum = calculate_checksum((uint8_t *)snd_buf, data_len);
	//snd_buf[data_len] = checksum;
	//
	//// g?i chu?i k�m checksum (data_len + 1 byte) qua i2c
	//I2C_Transmit(SLAVE_ADDRESS, (uint8_t *)snd_buf, data_len + 1);
//}



int main(void)
{
	USART_Init_Interrupt(MYUBRR);
	Servo_Init();
	Button_Init();
	I2C_Init();
	Alarm_Init();
	IR_Sensors_Init();
	HX711_init(128);
	_delay_ms(500);
	sei();
	
	USART_Send_String("Taring...\n");
	HX711_tare(100);    // Calibration
	USART_Send_String("Tare done.\n");
	
	HX711_set_scale(LOADCELL_SCALE);
	//HX711_set_offset(LOADCELL_OFFSET);
	
	//LED_On(LED_RED);
	
    /* Replace with your application code */
    while (1) 
    {	
		if (json_parsed_flag)
		{
			send_data_to_qt(); // G?i cho QT v� ESP qua I2C
			json_parsed_flag = 0;
		}

		while(power==0)
		{
			_delay_ms(1); // gi?m t?i CPU
		}
		if(rx_flag)
		{
			send_data_to_qt();
			rx_flag = 0;	
		}
		//uint32_t raw_adc = HX711_get_value(10);
		//memset(snd_buf, 0, sizeof(snd_buf));
		//sprintf(snd_buf, "raw adc = %lu\n", raw_adc);
		//USART_Send_String(snd_buf);	
		float raw_weight = HX711_get_units(1);
		if (raw_weight > MIN_WEIGHT) 
		{
			_delay_ms(200); // Cho c?m bi?n ?n ??nh sau khi c� v?t
			raw_weight = HX711_get_units(10); // L?y l?i d? li?u ch�nh x�c h?n
		}
		else continue;
		if (raw_weight < 0){
			 raw_weight = 0; 
		}
		weight = (uint16_t)raw_weight;
		memset(snd_buf, 0, sizeof(snd_buf));
		sprintf(snd_buf, "weight = %d\n", weight);
		USART_Send_String(snd_buf);	
		
		if (weight>=MIN_WEIGHT && weight<threshold_1)
		{
			if ((light_incont + weight) >= light_preset)
			{
				full = 1;
				LED_On(LED_RED);
				power=0;
				send_data_to_qt();
								
				while(power==0)
				{
					_delay_ms(1); // gi?m t?i CPU
				}
				
				/* sau khi chay lai */
				light_incont = 0;
				full = 0;
				LED_Off(LED_RED);
					
			}
			
			else
			{
				Servo3_Run();
				/*đợi đến khi IR sensor phát hiện sample đã đến gần servo*/
				while( IR_Sensor_Read(1) == 1);
				Servo1_SetAngle(SERVO_WORKING_ANGLE);
				_delay_ms(1000);// đợi thêm tí cho vật đi vào vùng gạt của servo
				Servo1_SetAngle(0);
				light_incont += weight;
				total_quantity++;
			}
			
		}
		else if(weight>=threshold_1 && weight<threshold_2)
		{
			
			if((medium_incont + weight)>=medium_preset)
			{
				full = 1;
				LED_On(LED_RED);
				power=0;
				send_data_to_qt();
				
				while(power==0)
				{
					_delay_ms(1); // gi?m t?i CPU
				}
				
				/* sau khi chay lai */
				medium_incont = 0;
				full = 0;
				LED_Off(LED_RED);
			}
			
			else
			{
				Servo3_Run();
				/*đợi đến khi IR sensor phát hiện sample đã đến gần servo*/
				while( IR_Sensor_Read(2) == 1);
				Servo2_SetAngle(SERVO_WORKING_ANGLE);
				_delay_ms(1000);// đợi thêm tí cho vật đi vào vùng gạt của servo
				Servo2_SetAngle(0);
				medium_incont += weight;
				total_quantity++;
			}
		}
		else if(weight>=threshold_2)
		{
			if((heavy_incont + weight) >= heavy_preset)
			{
				full = 1;
				LED_On(LED_RED);
				power=0;
				send_data_to_qt();
				while(power==0)
				{
					_delay_ms(1); // gi?m t?i CPU
				}
				
				/* sau khi chay lai */
				heavy_incont = 0;
				full = 0;
				LED_Off(LED_RED);
			}
			
			else
			{
				Servo3_Run();
				_delay_ms(1000);
				heavy_incont += weight;
				total_quantity++;
			}
		}
				
		send_data_to_qt(); 
		
	}
}

