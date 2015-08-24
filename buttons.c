#include "buttons.h"

struct BTN_struct {
	uint8_t cnt;	
};

static struct BTN_struct btns;

void BTN_init(){
	btns.cnt = 3;
	DDRD &= ~(_BV(BTN1) | _BV(BTN2) | _BV(BTN3));
	PORTD |= _BV(BTN1) | _BV(BTN2) | _BV(BTN3);

	//DEBUG
	DDRD |= _BV(LED);
	PORTD |= _BV(LED);
}

inline uint8_t pressed_check(){
	uint8_t rt = 0;
	uint8_t sum = _BV(BTN1) | _BV(BTN2) | _BV(BTN3);
	rt = PIND & sum;
	rt = ~rt;
	rt = rt & sum;
	return rt;
}

void BTN_check(){
	uint8_t pressed = pressed_check();
	if (pressed)
		PORTD &= ~_BV(LED);
	return;
}

