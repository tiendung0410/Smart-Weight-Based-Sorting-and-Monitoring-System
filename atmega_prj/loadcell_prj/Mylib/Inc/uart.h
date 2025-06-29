/*
 * uart.h
 *
 * Created: 28-Mar-25 8:46:28 PM
 *  Author: admin
 */ 

#ifndef UART_H_
#define UART_H_

#define RX_BUF_SIZE 100  // Kích th??c buffer nh?n UART

extern unsigned char rx_buf[RX_BUF_SIZE];  
extern volatile unsigned char rx_flag; 

// Kh?i t?o UART không s? d?ng ng?t
void USART_Init_noInterrupt(unsigned int ubrr);

// Kh?i t?o UART có s? d?ng ng?t
void USART_Init_Interrupt(unsigned int ubrr);

// G?i m?t byte qua UART
void USART_Transmit_byte(char data);

// G?i m?t chu?i qua UART
void USART_Send_String(const char *buf);

// Nh?n m?t byte t? UART (ch? d? li?u ??n)
unsigned char USART_Receive_byte(void);

void uart_rx_handle(void);



#endif /* UART_H_ */