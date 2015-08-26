#include "PWM.h"

void PWM_clear_on_compare_match()
{
	TCCR1A |= _BV(COM1B1);
	TCCR1A &= ~_BV(COM1B0);
}

void PWM_set_on_compare_match()
{
	TCCR1A |= _BV(COM1B1) | _BV(COM1B0);
}

/*
 * OMG, Atmegas do not have "turning off" feature like ARM cortexes have!
 */
void PWM_stupid_turn_off()
{	
	TCCR1B &= ~_BV(CS11);
	TIMSK &= ~_BV(OCIE1B);	
	TCCR1A &= ~(_BV(COM1B1) | _BV(COM1B0));
}

void PWM_stupid_turn_on(int8_t direction)
{	
	if (direction ==  1)
		PWM_clear_on_compare_match();
	else if (direction == -1)
		PWM_set_on_compare_match();
	TIMSK |= _BV(OCIE1B);
	ADC_val_nearby(SWITCH_LEVEL, DEVIATION);
	TCCR1B |= _BV(CS11);
}

void POT_init()
{
	/*MODE 5, look to the datasheet, Waveform generation mode, p. 98 */
	TCCR1A = _BV(WGM10);
	TCCR1B = _BV(WGM12);
	/*End of setting mode*/	
	OCR1B = 128;	
	PWM_stupid_turn_off();
	ADC_init();
}

