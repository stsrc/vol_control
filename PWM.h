#ifndef _PWM_H_
#define _PWM_H_

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "VOL.h"

void PWM_clear_on_compare_match();
void PWM_set_on_compare_match();
void PWM_stupid_turn_off();
void PWM_stupid_turn_on(int8_t direction);
void PWM_init();

#endif
