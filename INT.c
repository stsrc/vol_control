#include "INT.h"

//TODO: DO IT AS MACRO
uint8_t INT_as_rising_edge()
{
	if(MCUCR & _BV(ISC10))
		return 1;
	return 0;
}

void INT_turn_edge()
{
	MCUCR ^= _BV(ISC10);
	MCUCR |= _BV(ISC11);
}

void INT_on_falling_edge()
{
	MCUCR |= _BV(ISC11);
	MCUCR &= ~_BV(ISC10);
}

void INT_on_rising_edge()
{
	MCUCR |= _BV(ISC11) | _BV(ISC10);
}


