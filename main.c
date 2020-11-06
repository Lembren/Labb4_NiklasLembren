#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

#include "adc.h"
#include "gpio.h"
#include "i2c.h"
#include "serial.h"
#include "timer.h"
char buffer[6];


int main(void) {

	i2c_init();
	uart_init();
	sei();

	eeprom_write_byte(0x10, 'N');
	eeprom_wait_until_write_complete();
	eeprom_write_byte(0x11, 'I');
	eeprom_wait_until_write_complete();
	eeprom_write_byte(0x12, 'K');
	eeprom_wait_until_write_complete();
	eeprom_write_byte(0x13, 'L');
	eeprom_wait_until_write_complete();
	eeprom_write_byte(0x14, 'A');
	eeprom_wait_until_write_complete();
	eeprom_write_byte(0x15, 'S');
	eeprom_wait_until_write_complete();


	buffer[0] = eeprom_read_byte(0x10);
	buffer[1] = eeprom_read_byte(0x11);
	buffer[2] = eeprom_read_byte(0x12);
	buffer[3] = eeprom_read_byte(0x13);
	buffer[4] = eeprom_read_byte(0x14);
	buffer[5] = eeprom_read_byte(0x15);

	printf_P(PSTR("%s\n"), buffer);


	while (1) {

	}
	return 0;
}

