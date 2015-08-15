#include "timer2.h"

void timer2_reset()
{
	TIFR |= _BV(TOV2);
	TCNT2 = 0;
}

void timer2_init()
{
	/*clkio/64 (from prescaler)*/
	TCCR2 |= _BV(CS22);
}

uint8_t timer2_get_val(uint8_t *value)
{
	uint8_t rt = 0;
	ATOMIC_BLOCK(ATOMIC_FORCEON){
		*value = TCNT2;	
		/* overflow situation*/
		if (TIFR & _BV(TOV2))
			rt = 1;
		timer2_reset();
	}
	return rt;
}
