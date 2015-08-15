#ifndef _TIMER2_H_
#define _TIMER2_H_
#include <stdio.h>
#include <util/atomic.h>
#include <avr/interrupt.h>

void timer2_reset();
void timer2_init();
uint8_t timer2_get_val();

#endif
