#ifndef _INT0_H_
#define _INT0_H_

#include <stdio.h>
#include <avr/io.h>

inline void INT0_init()
{
	/*Any logical change generates an interrupt request*/
	MCUCR |= _BV(ISC00);
	MCUCR &= ~_BV(ISC01);
	GICR |= _BV(INT0);
}


ISR(INT0_vect){
	PWR_STAGE_SWITCH();	
}

#endif //_INT0_H_
