#ifndef _TIMER2_H_
#define _TIMER2_H_

#include <avr/io.h>
#include <util/atomic.h>

void timer2_reset();
void timer2_init();
uint8_t timer2_get_val();

#endif
