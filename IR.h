#ifndef _IR_H_
#define _IR_H_

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "macros.h"
#include "INT.h"
#include "timer2.h"
#include "VOL.h"

void IR_init();
uint8_t IR_get_toggle();
uint8_t IR_get_device();
uint8_t IR_get_instruction();
uint8_t IR_perform_action();
int8_t IR_test_if_shorter_gap();
uint8_t IR_case4_algorithm(int8_t shorter_gap);

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

#endif
