#ifndef _MACROS_H_
#define _MACROS_H_

#define CS PB0
#define UD PB2
#define REL_1 PB6
#define JACK_INT PD2
#define PWR_STAGE PD4
#define IR_REMOTE PD3
#define BTN2 PD5
#define BTN1 PD6
#define BTN3 PD7
#define LED PD4

#define PREAMP_CODE 0 /*like a TV, but it's only DIY-project, so doesn't metter.*/
#define CHANNEL_1 1
#define CHANNEL_2 2
#define VOL_INCREASE 16
#define VOL_DECREASE 17

#define CS_LOW() PORTB &= ~_BV(CS)
#define CS_HIGH() PORTB |= _BV(CS)

#define UD_LOW() PORTB &= ~_BV(UD)
#define UD_HIGH() PORTB |= _BV(UD)

#define LED_ON() PORTD |= _BV(LED)
#define LED_OFF() PORTD &= ~_BV(LED)
#define LED_SWITCH() PORTD ^= _BV(LED)

#define REL_1_1() PORTB |= _BV(REL_1)
#define REL_1_0() PORTB &= ~_BV(REL_1)
#define REL_SWITCH() PORTB ^= _BV(REL_1)

#define PWR_STAGE_OFF() PORTD &= ~_BV(PWR_STAGE)
#define PWR_STAGE_ON() PORTD |= _BV(PWR_STAGE)
#define PWR_STAGE_SWITCH() PORTD ^= _BV(PWR_STAGE)

#endif
