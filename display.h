/* (c) 2018 ukrkyi */
#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

#define NUM_ROW 5

extern uint8_t rows[NUM_ROW];

extern void display_init();

static inline void display_setpoint(uint8_t row, uint8_t column, uint8_t value)
{
	rows[row] = (rows[row] & ~(1 << column)) | (value << column);
}

#endif // DISPLAY_H
