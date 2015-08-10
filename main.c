#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#ifndef __AVR_ATmega8__
#define __AVR_ATmega8__ 
#endif

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#define CS PB0
#define UD PB2
#define REL_1 PB6
#define JACK_INT PD2
#define CUR_STAGE_MUTE PD4
#define BTN2 PD5
#define BTN1 PD6
#define BTN0 PD7

#define CS_LOW() PORTB &= ~_BV(CS)
#define CS_HIGH() PORTB |= _BV(CS)
#define UD_LOW() PORTB &= ~_BV(UD)
#define UD_HIGH() PORTB |= _BV(UD)
#define REL_1_1() PORTB |= _BV(REL_1)
#define REL_1_0() PORTB &= ~_BV(REL_1)

int8_t volume;
uint8_t steps;
int8_t direction;

/*
 * OMG, Atmegas do not have "turning off" feature like ARM cortexes have!
 */
void PWM_stupid_turn_off()
{
	TIMSK &= ~_BV(OCIE1B);
	TCCR1A &= ~(_BV(COM1B1) | _BV(COM1B0));
}

inline void PWM_clear_on_compare_match()
{
	TCCR1A |= _BV(COM1B1);
	TCCR1A &= ~_BV(COM1B0);
}

inline void PWM_set_on_compare_match()
{
	TCCR1A |= _BV(COM1B1) | _BV(COM1B0);
}

void PWM_stupid_turn_on(int8_t direction)
{
	TIMSK |= ~_BV(OCIE1B);
	if (direction > 0)
		PWM_clear_on_compare_match();
	else 
		PWM_set_on_compare_match();
	
}

void VOL_increase(uint8_t steps_vol)
{
	direction = 1;
	steps = steps_vol;
	UD_HIGH();
	CS_LOW();
	_delay_ms(10);
	PWM_stupid_turn_on(direction);
}

void VOL_decrease(uint8_t steps_vol)
{
	direction = -1;
	steps = steps_vol;
	UD_LOW();
	CS_LOW();
	_delay_ms(10);
	PWM_stupid_turn_on(direction);
}

void PWM_init()
{
	/*MODE 5, look to the datasheet, Waveform generation mode, p. 98 */
	TCCR1A |= _BV(WGM10);
	TCCR1B |= _BV(WGM12);
	/*End of setting mode*/
	TCCR1B |= _BV(CS11); //clkio/8 from prescaler	
	TIMSK |= _BV(OCIE1B); //output comapre B match interupt enable
	OCR1B = 128;
	PWM_stupid_turn_off();	
	sei();
}

void pins_init()
{
	DDRB = 0xff;
	PORTB = 0;
	DDRC = 0xff;
	PORTC = 0;
	DDRD = 0xff;
	PORTD = 0;

	DDRD &= ~(_BV(BTN0) | _BV(BTN1) | _BV(BTN2));
	PORTD |= _BV(BTN0) | _BV(BTN1) | _BV(BTN2);
	PORTD |= _BV(CUR_STAGE_MUTE);
	CS_HIGH();
}

int main(void)
{
	pins_init();
	PWM_init();
	VOL_increase(255);
	while(1){
	}
	return 0;
}

ISR(TIMER1_COMPB_vect)
{
	volume += direction;
	steps--;
	if(!steps) PWM_stupid_turn_off();
}
