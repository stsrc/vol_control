#ifndef _PWM_H_
#define _PWM_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#include "macros.h"
#include "ADC.h"

void PWM_clear_on_compare_match();
void PWM_set_on_compare_match();
void PWM_stupid_turn_off();
void PWM_stupid_turn_on(int8_t direction);
void POT_init();

#endif
