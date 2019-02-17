#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdint.h>

#include "display.h"
#include "timer.h"
#include "shift.h"

int main(){
	// Pull-up RESET pin to avoid eventual reset
	PORTB = 0x20;
	display_init();
	shift_init();
	timer_init();
	sei();
	while(1);
}

