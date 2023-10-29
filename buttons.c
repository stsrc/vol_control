#include "buttons.h"

struct BTN_struct {
	const uint8_t no;
	const uint8_t pins;
	/*
	 * XXX: if fclk is higher than 8MHz, think about changing counters size to 32 bits,
	 * because delay caused by debounce function may not be sufficient
	 * (if 2^32 is to long - adjust delay with modulo)
	 */
	uint16_t *cntrs;
};

static struct BTN_struct btns = {
	.no = 3,
	.pins = _BV(BTN1) | _BV(BTN2) | _BV(BTN3),
	.cntrs = NULL
};

void BTN_init()
{
	DDRD &= ~btns.pins;
	PORTD |= btns.pins;
	btns.cntrs = malloc(sizeof(uint8_t) * btns.no);
	if (btns.cntrs == NULL)
		while(1);
	for (uint8_t it = 0; it < btns.no; it++)
		btns.cntrs[it] = 0;
}

static uint8_t BTN_check()
{
	return ((~PIND) & btns.pins);
}

static uint8_t BTN_debounce_(uint16_t *cntr,
			     const uint8_t btn,
			     const uint8_t pressed)
{
	uint8_t rt = 0;
	if (*cntr) {
		(*cntr)++;
		if (!*cntr) {
			if (BTN_check() & _BV(btn)) {
				(*cntr)++;
			} else {
				rt |= _BV(btn);
			}
		}
	} else if (pressed & _BV(btn)) {
		(*cntr)++;
	}
	return rt;
}

static inline uint8_t BTN_debounce()
{
	uint8_t pressed = BTN_check();
	uint8_t rt = BTN_debounce_(&btns.cntrs[0], BTN1, pressed);
	rt |= BTN_debounce_(&btns.cntrs[1], BTN2, pressed);
	rt |= BTN_debounce_(&btns.cntrs[2], BTN3, pressed);
	return rt;
}

/*Place for functions which buttons may trigger*/
static inline void BTN_perform(uint8_t action)
{
	if (_BV(BTN1) & action) {
		VOL_decrease(1);
	}
	if (_BV(BTN2) & action) {
		VOL_increase(1);
	}
	if (_BV(BTN3) & action) {
		REL_SWITCH();
	}
}

void BTN_action()
{
	BTN_perform(BTN_debounce());
}

