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
#include <util/atomic.h>
#include <stdlib.h>

#include "timer2.h"
#include "INT.h"

#define CS PB0
#define UD PB2
#define REL_1 PB6
#define JACK_INT PD2
#define CUR_STAGE_MUTE PD4
#define IR_REMOTE PD3
#define BTN2 PD5
#define BTN1 PD6

#define LED PD7
#define LED_ON() PORTD |= _BV(LED)
#define LED_OFF() PORTD &= ~_BV(LED)
#define LED_SWITCH() PORTD ^= _BV(LED)

#define CS_LOW() PORTB &= ~_BV(CS)
#define CS_HIGH() PORTB |= _BV(CS)
#define UD_LOW() PORTB &= ~_BV(UD)
#define UD_HIGH() PORTB |= _BV(UD)
#define REL_1_1() PORTB |= _BV(REL_1)
#define REL_1_0() PORTB &= ~_BV(REL_1)

#define PREAMP_CODE 0 /*like a TV, but it's only DIY-project, so doesn't metter.*/
#define CHANNEL_1 1
#define CHANNEL_2 2
#define VOL_INCREASE 16
#define VOL_DECREASE 17

struct IR_struct{
	volatile uint8_t state;
	uint8_t time;
	const uint8_t time_limit;
	const uint8_t time_lower_limit;
	const uint8_t time_higher_limit;
	volatile uint8_t bit;
	volatile uint8_t ignore;
	volatile uint8_t data[14];
	const uint8_t bit_limit;
};

struct IR_struct IR = {
	.state = 0,
	.time = 0,
	/*
	 * 1 bit takes 1778us. Prescaler gives 8us per tick.
	 * One bit (1778us) => 222.25 ticks
	 * Half of bit (889us) => 111.125 ticks
	 * Let the limit be between those values: 1.5*889us = 1333.5us. 
	 * 1333.5us/8us = 166.6875 => 167
	 */
	.time_limit = 167,
	/*time_lower_limit - treshold for half bit time, equal to 0.5*time of halfbit.*/
	.time_lower_limit = 55,
	/*with overflow flag set*/
	.time_higher_limit = 78,
	.bit = 0,
	.ignore = 1,
	.bit_limit = 14
};
volatile int8_t volume = 0;
volatile int8_t steps = 0;
volatile int8_t direction = 0;

inline void PWM_clear_on_compare_match()
{
	TCCR1A |= _BV(COM1B1);
	TCCR1A &= ~_BV(COM1B0);
}

inline void PWM_set_on_compare_match()
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
	TCCR1B |= _BV(CS11);
}

void PWM_init()
{
	/*MODE 5, look to the datasheet, Waveform generation mode, p. 98 */
	TCCR1A = _BV(WGM10);
	TCCR1B = _BV(WGM12);
	/*End of setting mode*/	
	OCR1B = 128;	
	PWM_stupid_turn_off();
}

inline void VOL_stop_changing()
{
	PWM_stupid_turn_off();
	direction = 0; 
	CS_HIGH();
}

void VOL_increase(uint8_t steps_vol)
{
	PWM_stupid_turn_off();
	if (direction == -1){
		steps -= steps_vol;
		if (steps > 0) {
			PWM_stupid_turn_on(direction);
			return;
		} else if (!steps) {
			VOL_stop_changing();
			return;
		} else {
			steps = steps*(-1);
			CS_HIGH();
			_delay_ms(1);
		}
	} else if (direction == 1) {
		steps += steps_vol;
		if (steps < 0) 
			steps = 127;
		PWM_stupid_turn_on(direction);
		return;
	} else {	
		steps = steps_vol;
	}
	direction = 1;
	UD_HIGH();
	_delay_ms(1);
	CS_LOW();
	_delay_ms(1);
	PWM_stupid_turn_on(direction);
}

void VOL_decrease(uint8_t steps_vol)
{
	PWM_stupid_turn_off();
	if (direction == -1) {
		steps += steps_vol;
		if (steps < 0)
			steps = 127;
		PWM_stupid_turn_on(direction);
		return;
	} else if (direction == 1) {
		steps -= steps_vol;
		if (steps > 0) {
			PWM_stupid_turn_on(direction);
			return;
		} else if (!steps) {
			VOL_stop_changing();
			return;
		} else {
			steps = steps*(-1);
			CS_HIGH();
			_delay_ms(1);
		}
	} else {
		steps = steps_vol;
	}
	direction = -1;
	UD_LOW();
	_delay_ms(1);
	CS_LOW();
	_delay_ms(1);
	PWM_stupid_turn_on(direction);
}

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

void IR_init()
{
	DDRD &= ~_BV(IR_REMOTE);
	PORTD |= _BV(IR_REMOTE);
	timer2_init();
	INT_on_falling_edge();
	GICR |= _BV(INT1);
}

inline uint8_t IR_get_toggle()
{
	return IR.data[2];
}

inline uint8_t IR_get_device()
{
	uint8_t device = 0;
	for (uint8_t it = 3; it < 8; it++) {
		if (IR.data[it]) 
			device |= 1;
		if(it != 7)
			device = device << 1;
	}
	return device;
}

inline uint8_t IR_get_instruction()
{
	uint8_t instruction = 0;
	for (uint8_t it = 8; it < 14; it++) {
		if (IR.data[it])
			instruction |= 1;
		if (it != 13)
			instruction = instruction << 1;
	}
	return instruction;
}


uint8_t IR_perform_action(){
	uint8_t toggle, device, instruction;
	while(IR.ignore);
	IR.ignore = 1;
	toggle = IR_get_toggle();
	device = IR_get_device();
	instruction = IR_get_instruction();	
		if(device != PREAMP_CODE)
			return 1;
	switch(instruction){
	case CHANNEL_1:
		REL_1_1();
		LED_OFF();
		break;
	case CHANNEL_2:
		REL_1_0();
		LED_ON();
		break;
	case VOL_INCREASE:
		VOL_increase(1);
		break;
	case VOL_DECREASE:
		VOL_decrease(1);
		break;
	default:
		return 1;
		}
	return 0;
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

ISR(TIMER1_COMPB_vect, ISR_BLOCK)
{
	volume += direction;
	steps--;
	if (!steps)
		VOL_stop_changing();
}

inline int8_t IR_test_if_shorter_gap()
{
	if (timer2_get_val(&IR.time)) {
		if (IR.time > IR.time_higher_limit)
			return -1;
		else
			return 0;
	} else {
		if (IR.time > IR.time_limit)
			return 0;
		else if ((IR.time >= IR.time_lower_limit) 
				&& (IR.time <= IR.time_limit))
			return 1;
		else
			return -1;
	}
	return -1;
}

inline uint8_t case4_algorithm(int8_t shorter_gap)
{
	if (IR.data[IR.bit - 1] == 0){
		if (!INT_as_rising_edge()){
			if (shorter_gap){
				IR.data[IR.bit] = 0;
				IR.state = 5;
			} else {
				IR.data[IR.bit] = 1;
			}
		}
	} else {
		if (INT_as_rising_edge()){
			if(shorter_gap){
				IR.data[IR.bit] = 1;
				IR.state = 5;
			} else {
				IR.data[IR.bit] = 0;
			}
		}
	}
	IR.bit++;
	if(IR.bit == IR.bit_limit){
		return 1;
	}
	return 0;
}

ISR(INT1_vect, ISR_NOBLOCK)
{
	int8_t shorter_gap = IR_test_if_shorter_gap();
	if ((shorter_gap == -1) && IR.state)
		goto err;
	switch (IR.state) {
	case 0:
		IR.state = 1;
		IR.ignore = 1;
		IR.data[0] = 1;
		break;
	case 1:
		if (!shorter_gap)
			goto err;
		IR.state = 2;
		IR.data[1] = 1;
		break;
	case 2:
		if (!shorter_gap)
			goto err;
		IR.state = 3;
		break;	
	case 3:
		if (shorter_gap) {
			IR.data[2] = 1;
			IR.state = 5;
		} else {
			IR.data[2] = 0;
			IR.state = 4;
		}
		IR.bit = 3;
		break;
	case 4:
		if (case4_algorithm(shorter_gap)) {
			IR.ignore = 0;
			IR.state = 0;
			IR.bit = 0;
			INT_on_falling_edge();
			return;
		}
		break;
	case 5:
		IR.state = 4;
		break;
	default:
		goto err;
	}
	INT_turn_edge();
	return;
err:
	IR.state = 0;
	IR.bit = 0;
	IR.ignore = 1;
	INT_on_falling_edge();
	return;
}

