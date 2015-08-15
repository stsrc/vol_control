#include "VOL.h"


static volatile int8_t volume = 0;
static volatile int8_t steps = 0;
static volatile int8_t direction = 0;

void VOL_stop_changing()
{
	PWM_stupid_turn_off();
	direction = 0; 
	CS_HIGH();
}

void VOL_increase(uint8_t steps_vol)
{
	PWM_stupid_turn_off();
	if (direction == -1){
		steps -= steps_vol;
		if (steps > 0) {
			PWM_stupid_turn_on(direction);
			return;
		} else if (!steps) {
			VOL_stop_changing();
			return;
		} else {
			steps = steps*(-1);
			CS_HIGH();
			_delay_ms(1);
		}
	} else if (direction == 1) {
		steps += steps_vol;
		if (steps < 0) 
			steps = 127;
		PWM_stupid_turn_on(direction);
		return;
	} else {	
		steps = steps_vol;
	}
	direction = 1;
	UD_HIGH();
	_delay_ms(1);
	CS_LOW();
	_delay_ms(1);
	PWM_stupid_turn_on(direction);
}

void VOL_decrease(uint8_t steps_vol)
{
	PWM_stupid_turn_off();
	if (direction == -1) {
		steps += steps_vol;
		if (steps < 0)
			steps = 127;
		PWM_stupid_turn_on(direction);
		return;
	} else if (direction == 1) {
		steps -= steps_vol;
		if (steps > 0) {
			PWM_stupid_turn_on(direction);
			return;
		} else if (!steps) {
			VOL_stop_changing();
			return;
		} else {
			steps = steps*(-1);
			CS_HIGH();
			_delay_ms(1);
		}
	} else {
		steps = steps_vol;
	}
	direction = -1;
	UD_LOW();
	_delay_ms(1);
	CS_LOW();
	_delay_ms(1);
	PWM_stupid_turn_on(direction);
}


ISR(TIMER1_COMPB_vect, ISR_BLOCK)
{
	volume += direction;
	steps--;
	if (!steps)
		VOL_stop_changing();
}
