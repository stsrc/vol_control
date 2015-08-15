#ifndef _VOL_H_
#define _VOL_H_

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "macros.h"
#include "PWM.h"

void VOL_stop_changing();
void VOL_increase(uint8_t steps_vol);
void VOL_decrease(uint8_t steps_vol);

#endif
