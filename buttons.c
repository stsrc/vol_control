#include "buttons.h"

struct BTN_struct {
	const uint8_t cnt;
	const uint8_t pins;
	uint8_t pressed;
	uint8_t held;	
};

static struct BTN_struct btns = {
	.cnt = 3,
	.pins = _BV(BTN1) | _BV(BTN2) | _BV(BTN3)
};

void BTN_init()
{
	DDRD &= ~btns.pins;
	PORTD |= btns.pins;

	//DEBUG
	DDRD |= _BV(LED);
	LED_ON();
}

static inline uint8_t BTN_check()
{
	return ((~PIND) & btns.pins);
}

static inline void BTN_debounce()
{
		
}

/*Place for functions which buttons may trigger*/
static inline void BTN_perform()
{
	if (_BV(BTN1) & btns.pressed & (~btns.held)) {
		LED_OFF();
		btns.pressed &= ~_BV(BTN1);
	}
	if (_BV(BTN2) & btns.pressed & (~btns.held)) {
		LED_ON();
		btns.pressed &= ~_BV(BTN2);
	}
	if (_BV(BTN3) & btns.pressed & (~btns.held)) {
		LED_OFF();
		btns.pressed &= ~_BV(BTN3);
	}
}

void BTN_action()
{
	btns.pressed |= BTN_check();
	if (btns.pressed) {
		BTN_debounce();
		btns.held = BTN_check() & btns.pressed;
		BTN_perform();
	}
}

