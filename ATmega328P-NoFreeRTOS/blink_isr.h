#ifndef BLINK_ISR_H
#define BLINK_ISR_H
#include <avr/sfr_defs.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
void blink_isr_enable(void);
#endif /* BLINK_ISR_H */
