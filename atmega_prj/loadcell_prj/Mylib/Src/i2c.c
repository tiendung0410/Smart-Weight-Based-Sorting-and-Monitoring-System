/*
 * i2c.c
 *
 * Created: 28-Mar-25 11:38:56 PM
 *  Author: admin
 */ 

#include "i2c.h"

void I2C_Init()
{
	// T?nh to?n gi? tr? TWBR
	uint8_t twbr_value = (F_CPU / (2 * SCL_FREQUENCY)) - 8;

	TWBR = twbr_value; // C?i ??t TWI Bit Rate Register
	TWSR = 0x00;       // Prescaler = 1 (m?c ??nh)

	// C?u hình chân SDA và SCL làm input và b?t pull-up
	DDRC &= ~((1 << PINC0) | (1 << PINC1));       // PC0 = SCL, PC1 = SDA
	//PORTC |= (1 << PINC0) | (1 << PINC1);         // B?t pull-up n?i
	// K?ch ho?t TWI
	TWCR = (1 << TWEN);
}

uint8_t I2C_Transmit(uint8_t slave_address, const uint8_t *data, uint8_t length) {
	if (length == 0) {
		return 0; // Kh?ng c? d? li?u ?? truy?n
	}

	// G?i t?n hi?u START
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT))); // Ch? START ho?n t?t

	// Ki?m tra tr?ng th?i START
	if ((TWSR & 0xF8) != 0x08) {
		return 0; // L?i START
	}

	// G?i ??a ch? slave (bao g?m bit ??c/ghi)
	TWDR = (slave_address << 1) | 0; // Bit 0 = 0 (ghi)
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT))); 

	// Ki?m tra tr?ng th?i ACK
	if ((TWSR & 0xF8) != 0x18) {
		return 0; 
	}

	// Truy?n d? li?u
	for (uint8_t i = 0; i < length; i++) {
		TWDR = data[i]; 
		TWCR = (1 << TWINT) | (1 << TWEN);
		while (!(TWCR & (1 << TWINT)));

		// Ki?m tra tr?ng th?i ACK
		if ((TWSR & 0xF8) != 0x28) {
			return 0;
		}
	}

	// G?i t?n hi?u STOP
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
	while (TWCR & (1 << TWSTO)); // Ch? STOP ho?n t?t

	return 1; // Th?nh c?ng
}

uint8_t I2C_Receive(uint8_t slave_address, uint8_t *buffer, uint8_t length) {
	if (length == 0) {
		return 0; // Kh?ng c? d? li?u ?? nh?n
	}

	// G?i t?n hi?u START
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT))); // Ch? START ho?n t?t

	// Ki?m tra tr?ng th?i START
	if ((TWSR & 0xF8) != 0x08) {
		return 0; // L?i START
	}

	// G?i ??a ch? slave (bao g?m bit ??c/ghi)
	TWDR = (slave_address << 1) | 1; // Bit 0 = 1 (??c)
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT))); // Ch? truy?n ??a ch? ho?n t?t

	// Ki?m tra tr?ng th?i ACK
	if ((TWSR & 0xF8) != 0x40) {
		return 0; // L?i ??a ch? ho?c kh?ng nh?n ???c ACK
	}

	// Nh?n d? li?u
	for (uint8_t i = 0; i < length; i++) {
		if (i == length - 1) {
			// Byte cu?i c?ng: G?i NACK
			TWCR = (1 << TWINT) | (1 << TWEN);
			} else {
			// C?c byte tr??c: G?i ACK
			TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
		}

		while (!(TWCR & (1 << TWINT))); // Ch? nh?n d? li?u

		// Ki?m tra tr?ng th?i nh?n
		if ((TWSR & 0xF8) != (i == length - 1 ? 0x58 : 0x50)) {
			return 0; // L?i nh?n d? li?u
		}

		buffer[i] = TWDR; // L?u d? li?u v?o buffer
	}

	// G?i t?n hi?u STOP
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
	while (TWCR & (1 << TWSTO)); // Ch? STOP ho?n t?t

	return 1; // Th?nh c?ng
}