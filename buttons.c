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

static inline uint8_t BTN_check()
{
	return ((~PIND) & btns.pins);
}

static inline uint8_t BTN_debounce()
{
	uint8_t rt = 0;
	uint8_t pressed = BTN_check();
	if (btns.cntrs[0]) {
		btns.cntrs[0]++;
		if (!btns.cntrs[0]) {
			if (BTN_check() & _BV(BTN1))
				btns.cntrs[0]++;
			else
				rt |= _BV(BTN1);
		}
	} else if (pressed & _BV(BTN1)) {
		btns.cntrs[0]++;
	}
	if (btns.cntrs[1]) {
		btns.cntrs[1]++;
		if (!btns.cntrs[1]) {
			if (BTN_check() & _BV(BTN2))
				btns.cntrs[1]++;
			else
				rt |= _BV(BTN2);
		}
	} else if (pressed & _BV(BTN2)) {
		btns.cntrs[1]++;
	}
	if (btns.cntrs[2]) {
		btns.cntrs[2]++;
		if (!btns.cntrs[2]) {
			if (BTN_check() & _BV(BTN3))
				btns.cntrs[2]++;
			else
				rt |= _BV(BTN3);
		}
	} else if (pressed & _BV(BTN2)) {
		btns.cntrs[2]++;
	}
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

