/* (c) 2018 ukrkyi */

/*
 * Pinout:
 *
 * B4 -> CS
 * B3 -> Serial Data Output
 *
 * TIMER OUTPUT A as SCL
 *
 */

#include "display.h"
#include "timer.h"

#include <avr/io.h>

#define NUM_COLUMN	8U

#define TRANSMIT_SIZE (NUM_COLUMN*16U)

#define FIX_IGNORE_ROWS 1
#define CONFIG_BIT_NUM (8 + FIX_IGNORE_ROWS)

uint8_t rows[NUM_ROW] = {0};//0x1, 0x2, 0x3, 0x4, 0x5};
/*				test  on  | brightness   | line number     | shutdown| test off*/
static const uint8_t config[] = {0xF, 0x01, /*0x0A, 0x0,*/ 0xB, NUM_COLUMN - 1, 0xc, 0x1, 0xF, 0x00};
static const uint8_t conf_len = sizeof(config) * 8;

static uint8_t ptr = 0;

static inline void cs_high();
static inline void cs_low();

static void falling_it_config();
static void falling_it_data();

void display_init()
{
	falling_it = falling_it_config;
	cs_high();
	DDRB |= (1 << PIN4) | (1 << PIN3);
	// todo
}

static inline void set_data(uint8_t bit) {
	PORTB = (PORTB &~(1 << 3)) | ((bit != 0) << 3);
}

static inline uint8_t get_cs() {
	return PINB & (1<<PIN4);
}

void cs_high() {
	PORTB |= 1 << PIN4;
}

void cs_low() {
	PORTB &= ~(1 << PIN4);
}

void falling_it_data() {
	if (!(ptr % 16)) {
		if (!get_cs()) {
			cs_high();
			if (ptr == TRANSMIT_SIZE)
				ptr = 0;
			return;
		} else
			cs_low();
	}
	// Put next bit
	register uint8_t bit = 0;
	register uint8_t pos = ptr % 16;
	if (pos >= CONFIG_BIT_NUM + NUM_ROW)
		bit = 0;
	else if (pos >= CONFIG_BIT_NUM)
		bit = rows[pos - CONFIG_BIT_NUM] & (1 << (ptr / 16));
	else if (pos >= 4)
		bit = ((ptr / 16) + 1) & (1 << (7 - pos));
	set_data(bit);
	ptr++;
}

void falling_it_config() {
	if (!(ptr % 16)) {
		if (!get_cs()) {
			cs_high();
			if (ptr == conf_len) {
				ptr = 0;
				falling_it = falling_it_data;
			}
			return;
		} else
			cs_low();
	}
	// Put next bit
	set_data(config[ptr / 8]&(1 << (7 - ptr % 8)));
	ptr++;
}
