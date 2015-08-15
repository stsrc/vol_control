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
	DDRD &= ~(_BV(BTN1) | _BV(BTN2));
	DDRD &= ~_BV(IR_REMOTE);

	DDRD |= _BV(LED);

	PORTD |= _BV(BTN1) | _BV(BTN2);
	PORTD |= _BV(CUR_STAGE_MUTE);
	CS_HIGH();
}

int main(void)
{
	pins_init();
	PWM_init();
	IR_init();
	sei();
	VOL_increase(0x3F);
	LED_ON();
	while(1){
		IR_perform_action();		
	}
	return 0;
}

