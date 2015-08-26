#ifndef _ADC_H_
#define _ADC_H_

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void ADC_init();
uint8_t ADC_val_nearby(float val, float deviation);

#endif
