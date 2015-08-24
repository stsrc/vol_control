#ifndef _INT_H_
#define _INT_H_

#include <stdio.h>
#include <avr/io.h>

inline uint8_t INT_as_rising_edge()
{
	if(MCUCR & _BV(ISC10))
		return 1;
	return 0;
}

inline void INT_turn_edge()
{
	MCUCR ^= _BV(ISC10);
	MCUCR |= _BV(ISC11);
}

inline void INT_on_falling_edge()
{
	MCUCR |= _BV(ISC11);
	MCUCR &= ~_BV(ISC10);
}

inline void INT_on_rising_edge()
{
	MCUCR |= _BV(ISC11) | _BV(ISC10);
}

inline void INT_init()
{
	MCUCR |= _BV(ISC11);	
}

#endif
