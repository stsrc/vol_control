#include "buttons.h"

struct BTN_struct {
	uint8_t cnt;
	uint8_t pins;	
};

static struct BTN_struct btns;

void BTN_init(){
	btns.cnt = 3;
	btns.pins = _BV(BTN1) | _BV(BTN2) | _BV(BTN3);
	DDRD &= ~btns.pins;
	PORTD |= btns.pins;

	//DEBUG
	DDRD |= _BV(LED);
	LED_ON();
}

void BTN_check(){
	uint8_t pressed = (~PIND) & btns.pins;
	if (pressed)
		LED_OFF();
	return;
}

