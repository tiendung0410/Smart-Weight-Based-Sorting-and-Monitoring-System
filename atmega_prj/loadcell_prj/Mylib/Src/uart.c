#include <avr/io.h>
#include <avr/interrupt.h>

#define RX_BUF_SIZE 150

unsigned char rx_buf[RX_BUF_SIZE];
volatile unsigned char idx = 0;
volatile unsigned char rx_flag = 0;

/* Kh?i t?o UART không ng?t */
void USART_Init_noInterrupt(unsigned int ubrr)
{
	// ??t baud rate
	UBRRH = (unsigned char)(ubrr >> 8);
	UBRRL = (unsigned char)ubrr;
	
	// C?u hình ch? ?? UART
	UCSRA = 0;  // Normal speed
	UCSRB = (1 << RXEN) | (1 << TXEN);  // B?t Tx, Rx, không b?t ng?t
	UCSRC = (1 << URSEL) | (3 << UCSZ0); // 8-bit, 1 stop, no parity
}

/* Kh?i t?o UART có ng?t */
void USART_Init_Interrupt(unsigned int ubrr)
{
	// ??t baud rate
	UBRRH = (unsigned char)(ubrr >> 8);
	UBRRL = (unsigned char)ubrr;
	
	// C?u hình ch? ?? UART
	UCSRA = 0;  // Normal speed
	UCSRB = (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);  // B?t Tx, Rx, b?t ng?t RX
	UCSRC = (1 << URSEL) | (3 << UCSZ0); // 8-bit, 1 stop, no parity
}

void uart_rx_handle(void)
{
	unsigned char received = UDR;

	if (received == '}')  // K?t thúc chu?i
	{
		rx_buf[idx++] = '}';
		rx_buf[idx] = '\0';  // Thêm null-terminator
		rx_flag = 1;         // Báo có d? li?u m?i
		idx = 0;             // Reset index cho buffer
	}
	else
	{
		if (idx < RX_BUF_SIZE - 1)  // Ki?m tra tràn b? ??m
		{
			rx_buf[idx++] = received;
		}
	}
}

/* Truy?n 1 byte */
void USART_Transmit_byte(char data)
{
	while (!(UCSRA & (1 << UDRE)));  
	UDR = data;
}

/* Truy?n 1 chu?i */
void USART_Send_String(const char *buf)
{
	while (*buf)  // L?p ??n khi g?p ký t? k?t thúc chu?i '\0'
	{
		USART_Transmit_byte(*buf++);
	}
}

/* Nh?n 1 byte */
unsigned char USART_Receive_byte(void)
{
	while (!(UCSRA & (1 << RXC)));  // ??i khi có d? li?u
	return UDR;
}