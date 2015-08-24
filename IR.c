#include "IR.h"

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

static struct IR_struct IR = {
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

void IR_init()
{
	DDRD &= ~_BV(IR_REMOTE);
	PORTD |= _BV(IR_REMOTE);
	timer2_init();
	INT_on_falling_edge();
	GICR |= _BV(INT1);
}

static inline uint8_t IR_get_toggle()
{
	return IR.data[2];
}

static inline uint8_t IR_get_device()
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

static inline uint8_t IR_get_instruction()
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

uint8_t IR_action()
{
	uint8_t toggle, device, instruction;
	if(IR.ignore)
		return 0;
	IR.ignore = 1;
	toggle = IR_get_toggle();
	device = IR_get_device();
	instruction = IR_get_instruction();	
		if(device != PREAMP_CODE)
			return 1;
	switch(instruction){
	case CHANNEL_1:
		REL_1_1();
		break;
	case CHANNEL_2:
		REL_1_0();
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


static inline int8_t IR_test_if_shorter_gap()
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

static inline uint8_t IR_case4_algorithm(int8_t shorter_gap)
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
		if (IR_case4_algorithm(shorter_gap)) {
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
