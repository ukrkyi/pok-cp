/* (c) 2018 ukrkyi */

#include "timer.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "shift.h"

void timer_init() {
	//TIFR0 = (1 << OCF0A) | (1 << TOV0); // clear interrupts
	DDRB |= 1 << PIN0;
	TCCR0A = (0x3 << COM0A0) | 0x3; // set output A on compare match, clear at TOP
	OCR0A  = 0x7F;
	TIMSK0 = 0x6; // Overflow IT & CMPA IT
	TCCR0B = 0x2; // Start timer with division by 8
}

void (*falling_it)(void);

ISR(TIM0_COMPA_vect) {
	rising_it();
}

ISR(TIM0_OVF_vect) {
	falling_it();
}
