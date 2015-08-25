#ifndef _HDPHNS_H_
#define _HDPHNS_H_

#include <stdio.h>
#include <avr/io.h>

volatile uint16_t debounce = 0;

uint8_t HD_check(){
	return (PIND & _BV(JACK_INT));
}

void HD_task()
{
	if (!HD_check()) {
		PWR_STAGE_OFF();
		MCUCR |= _BV(ISC00);
	} else {
		PWR_STAGE_ON();
		MCUCR &= ~_BV(ISC00);
	}
	GICR |= _BV(INT0);
}

void HDPHNS_init()
{
	DDRD &= ~_BV(JACK_INT);
	PORTD &= ~_BV(JACK_INT);
	MCUCR |= _BV(ISC01);
	HD_task();
}

void HDPHNS_action()
{
	if (debounce) {
		debounce++;
		if (!debounce)
			HD_task();
	}
}

ISR(INT0_vect)
{
	debounce++;
	GICR &= ~_BV(INT0);
}

#endif
