/* (c) 2018 ukrkyi */

#include "shift.h"
#include "display.h"

#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>

#define SHIFT_SIZE ((uint8_t)(8 * NUM_ROW))

static inline void pl_high();
static inline void pl_low();

static uint8_t ptr;

void shift_init() {
	pl_high();
	DDRB |= (1 << PIN1);
	DDRB &= ~(1 << PIN2);
}

void pl_high() {
	PORTB |= 1 << PIN1;
}

void pl_low() {
	PORTB &= ~(1 << PIN1);
}

static inline uint8_t get_pl() {
	return PINB & (1<<PIN1);
}

static inline uint8_t get_data() {
	return !!(PINB & (1<<PIN2));
}

void rising_it() {
	if (ptr == SHIFT_SIZE) {
		if (get_pl()) {
			pl_low();
			return;
		} else {
			pl_high();
			ptr = 0;
		}
	}
	display_setpoint(ptr / 8, ptr % 8, get_data());
	ptr++;
}
