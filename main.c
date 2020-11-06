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


int main(void) {

	i2c_init();
	uart_init();
	sei();

	char buffer[10];

	eeprom_read_byte(0x70);
	_delay_ms(1000);
	eeprom_read_byte(0x70);

	while (1) {

	}
	return 0;
}

