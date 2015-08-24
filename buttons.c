#include "buttons.h"

struct BTN_struct {
	const uint8_t cnt;
	const uint8_t pins;
	uint8_t pressed;	
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

static inline void BTN_check()
{
	btns.pressed = (~PIND) & btns.pins;
}

static inline void BTN_debounce()
{
	/*button debounce*/
	for (uint8_t it = 1; it != 0; it++);
}

static inline void BTN_perform()
{
	if (_BV(BTN1) & btns.pressed)
		LED_OFF();
	if (_BV(BTN2) & btns.pressed)
		LED_ON();
	if (_BV(BTN3) & btns.pressed)
		LED_OFF();
}

void BTN_action()
{
	BTN_check();
	if (btns.pressed) {
		BTN_debounce();
		BTN_perform();
		btns.pressed = 0;
	}
}

