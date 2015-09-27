#ifndef _IR_H_
#define _IR_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "macros.h"
#include "INT1.h"
#include "timer2.h"
#include "VOL.h"

void IR_init();
uint8_t IR_action();
#endif
