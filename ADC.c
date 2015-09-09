#include "ADC.h"

static volatile uint16_t ADC_val = 0;
static volatile uint8_t ADC_cnt = 0;
static volatile uint16_t ADC_result = 0;

void ADC_init()
{
	/*Vref = AVCC*/
	ADMUX |= _BV(REFS0);
	/*MUX3..0 = ADC6*/
	ADMUX |= _BV(MUX2) | _BV(MUX1);
	/*ADC interrupt enable*/
	ADCSRA |= _BV(ADIE);
	/*ADC enabled*/
	ADCSRA |= _BV(ADEN);
	/*ADC frequency div. factor = 64*/
	ADCSRA |= _BV(ADPS2) | _BV(ADPS1);
	/*ADC start conversion*/
	ADCSRA |= _BV(ADSC);
}

uint8_t ADC_val_nearby(float val, float deviation){
	uint16_t lower = (uint16_t)(val - deviation * val);
	uint16_t higher = (uint16_t)(val + deviation * val);
	uint16_t ADC_at_res = ADC_result;
	if (ADC_at_res > higher || ADC_at_res < lower)
		return 0;
	else
		return 1;
	return 0;
}

ISR(ADC_vect){
	ADC_val += ADC;
	ADC_cnt++;
	if(ADC_cnt == 2){
		ADC_cnt = 0;
		ADC_result = ADC_val / 2;
		ADC_val = 0;
	}
	ADMUX ^= _BV(MUX0);
	ADCSRA |= _BV(ADSC);
}
