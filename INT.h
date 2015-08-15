#ifndef _INT_H_
#define _INT_H_

#include <stdio.h>
#include <avr/io.h>

uint8_t INT_as_rising_edge();
void INT_turn_edge();
void INT_on_falling_edge();
void INT_on_rising_edge();

#endif
