#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "macros.h"
#include "IR.h"

void pins_init()
{
	DDRB = 0xff;
	PORTB = 0;
	DDRC = 0xff;
	PORTC = 0;
	DDRD = 0xff;
	PORTD = 0;
	DDRD &= ~(_BV(BTN1) | _BV(BTN2) _BV(BTN3));
	DDRD &= ~_BV(IR_REMOTE);
	PORTD |= _BV(BTN1) | _BV(BTN2) | _BV(BTN3);
	PORTD |= _BV(CUR_STAGE_MUTE);
	CS_HIGH();
}

int main(void)
{
	pins_init();
	PWM_init();
	IR_init();
	sei();
	VOL_decrease(0x3F);
	while(1){
		IR_perform_action();		
	}
	return 0;
}

