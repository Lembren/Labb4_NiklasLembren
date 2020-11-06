#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include <stdio.h>
#include <string.h>

#include "i2c.h"

void i2c_init(void) {
	TWCR = (1 << TWEN);
	TWBR = 72;  //TWBR  = ((16000000 / 100000) – 16) / 2
	TWSR = 0;   //prescaler 1
}

void i2c_meaningful_status(uint8_t status) {
	switch (status) {
		case 0x08: // START transmitted, proceed to load SLA+W/R
			printf_P(PSTR("START\n"));
			break;
		case 0x10: // repeated START transmitted, proceed to load SLA+W/R
			printf_P(PSTR("RESTART\n"));
			break;
		case 0x38: // NAK or DATA ARBITRATION LOST
			printf_P(PSTR("NOARB/NAK\n"));
			break;
		// MASTER TRANSMIT
		case 0x18: // SLA+W transmitted, ACK received
			printf_P(PSTR("MT SLA+W, ACK\n"));
			break;
		case 0x20: // SLA+W transmitted, NAK received
			printf_P(PSTR("MT SLA+W, NAK\n"));
				break;
		case 0x28: // DATA transmitted, ACK received
			printf_P(PSTR("MT DATA+W, ACK\n"));
			break;
		case 0x30: // DATA transmitted, NAK received
			printf_P(PSTR("MT DATA+W, NAK\n"));
			break;
		// MASTER RECEIVE
		case 0x40: // SLA+R transmitted, ACK received
			printf_P(PSTR("MR SLA+R, ACK\n"));
			break;
		case 0x48: // SLA+R transmitted, NAK received
			printf_P(PSTR("MR SLA+R, NAK\n"));
			break;
		case 0x50: // DATA received, ACK sent
			printf_P(PSTR("MR DATA+R, ACK\n"));
			break;
		case 0x58: // DATA received, NAK sent
			printf_P(PSTR("MR DATA+R, NAK\n"));
			break;
		default:
			printf_P(PSTR("N/A %02X\n"), status);
			break;
	}
}

inline void i2c_start() {
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); // send START condition
	while (!(TWCR & (1 << TWINT))); // wait until TWINT flag set
}

inline void i2c_stop() {
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN); // send stop condition 
	while (TWCR & (1 << TWSTO)); // wait until stop condition is executed and bus released
}

inline uint8_t i2c_get_status(void) {

	uint8_t status;
	status = (TWSR & 0xF8); // Return TWI Status Register, mask the prescaler bits
	return status;
}

inline void i2c_xmit_addr(uint8_t address, uint8_t rw) {
	TWDR = (address & 0xfe) | (rw & 0x01); //// Send slave address (SLA_W)
	TWCR = (1 << TWINT) | (1 << TWEN); // Send Data
	while (!(TWCR & (1 << TWINT)));
}
inline void i2c_xmit_byte(uint8_t data) {
	TWDR = data;   // Put data into data register and start transmission
	TWCR = (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT)));
}

inline uint8_t i2c_read_ACK() {
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	while (!(TWCR & (1 << TWINT)));

	return TWDR;
}

inline uint8_t i2c_read_NAK() {

	TWCR = (1 << TWEN) | (1 << TWINT);	/* Enable TWI and clear interrupt flag */
	while (!(TWCR & (1 << TWINT)));	/* Wait until TWI finish its current job */
	return TWDR;		/* Return received data */

}

inline void eeprom_wait_until_write_complete() {
	while (i2c_get_status() != 0x18) { //SLA+W has been transmitted; ACK has been received
		i2c_start();
		i2c_xmit_addr(0xA0, 0); //Device Identifier, Device Address
	}
}

uint8_t eeprom_read_byte(uint8_t addr) {
	uint8_t status_read;

	i2c_start();
	//i2c_meaningful_status(i2c_get_status());


	i2c_xmit_addr(0xA0, 0); //adress + write
	//i2c_meaningful_status(i2c_get_status());

	i2c_xmit_byte(addr);
	//i2c_meaningful_status(i2c_get_status());

	i2c_start();
	i2c_xmit_addr(0xA0, 1); //adress + read
	//i2c_meaningful_status(i2c_get_status());

	status_read = i2c_read_NAK();

	//i2c_meaningful_status(i2c_get_status());

	i2c_stop();
	return status_read;
}

void eeprom_write_byte(uint8_t addr, uint8_t data) {
	i2c_start();
	//i2c_meaningful_status(i2c_get_status());
	i2c_xmit_addr(0xA0, 0);  // Send controll adress
	//i2c_meaningful_status(i2c_get_status());
	i2c_xmit_byte(addr);				//storage adress
	//i2c_meaningful_status(i2c_get_status());
	i2c_xmit_byte(data);				// send data
	//i2c_meaningful_status(i2c_get_status());
	i2c_stop();
}



void eeprom_write_page(uint8_t addr, uint8_t *data) {
	// ... (VG)
}

void eeprom_sequential_read(uint8_t *buf, uint8_t start_addr, uint8_t len) {
	// ... (VG)
}
