#ifndef _INT_H_
#define _INT_H_

#include <avr/interrupt.h>

uint8_t INT_as_rising_edge();
void INT_turn_edge();
void INT_on_falling_edge();
void INT_on_rising_edge();

#endif
