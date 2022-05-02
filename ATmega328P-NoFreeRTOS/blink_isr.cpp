/*
  Blink LED util from ATmega328P
  Refer to <avrfreertos/UnoBlink/main.c> for the IO control
  and <miniAVRfreeRTOS/port.c> for the watchdog setup
*/
#include "blink_isr.h"
#include <avr/sfr_defs.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
// Toggle light
ISR(WDT_vect, ISR_NOBLOCK) {
  static unsigned char led_on;
  if (led_on)
    PORTB |= _BV(PORTB5);
  else
    PORTB &= ~_BV(PORTB5);
  led_on ^= 1;
}
// Enable the watchdog timer interrupt without enabling the reset
// Refer to <avr/wdt.h>
static __inline__ __attribute__ ((__always_inline__))
void wdt_interrupt_enable (const uint8_t value) {
  if (_SFR_IO_REG_P (_WD_CONTROL_REG)) {
    __asm__ __volatile__ (
    "in __tmp_reg__,__SREG__"   "\n\t"
    "cli"                       "\n\t"
    "wdr"                       "\n\t"
    "out %0, %1"                "\n\t"
    "out __SREG__,__tmp_reg__"  "\n\t"
    "out %0, %2"                "\n\t"
    : /* no outputs */
    : "I" (_SFR_IO_ADDR(_WD_CONTROL_REG)),
    "r" ((uint8_t)(_BV(_WD_CHANGE_BIT) | _BV(WDE))),
    "r" ((uint8_t) ((value & 0x08 ? _WD_PS3_MASK : 0x00) |
    _BV(WDIF) | _BV(WDIE) | (value & 0x07)) )
    : "r0"
    );
  } else {
    __asm__ __volatile__ (
    "in __tmp_reg__,__SREG__"   "\n\t"
    "cli"                       "\n\t"
    "wdr"                       "\n\t"
    "sts %0, %1"                "\n\t"
    "out __SREG__,__tmp_reg__"  "\n\t"
    "sts %0, %2"                "\n\t"
    : /* no outputs */
    : "n" (_SFR_MEM_ADDR(_WD_CONTROL_REG)),
    "r" ((uint8_t)(_BV(_WD_CHANGE_BIT) | _BV(WDE))),
    "r" ((uint8_t) ((value & 0x08 ? _WD_PS3_MASK : 0x00) |
    _BV(WDIF) | _BV(WDIE) | (value & 0x07)) )
    : "r0"
    );
  }
}
void blink_isr_enable(void) {
  DDRB |= _BV(DDB5);
  wdt_interrupt_enable(WDTO_500MS); 
}
