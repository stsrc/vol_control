#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "macros.h"
#include "IR.h"
#include "INT0.h"
#include "buttons.h"

void pins_init()
{
	DDRB = 0xff;
	PORTB = 0;
	DDRC = 0xff;
	PORTC = 0;
	DDRD = 0xff;
	PORTD = 0;

	REL_1_1();
	PWR_STAGE_ON();
	DDRD &= ~(_BV(BTN1) | _BV(BTN2) | _BV(BTN3));
	DDRD &= ~_BV(IR_REMOTE); //input without pull-up
	DDRD &= ~_BV(JACK_INT); //input without pull-up
	PORTD |= _BV(BTN1) | _BV(BTN2) | _BV(BTN3);
	CS_HIGH();
}

int main(void)
{
	pins_init();
	PWM_init();
	INT0_init();
	IR_init();
	BTN_init();
	sei();
	VOL_decrease(0x3F);
	while(1){
		IR_action();
		BTN_action();	
	}
	return 0;
}

