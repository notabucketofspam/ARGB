/*
  FreeRTOS proved to be too clunky in this instance, as the frametime increases
  by 15ms with each added task, making precise control of the LEDs impossible.
*/
#include <stdint.h>
#include "blink_isr.h"
// micros() in ArduinoCore still breaks even without FreeRTOS, so use avr-libc
// delay utils instead
#include <util/delay.h>
#define NO_MINIMUM_WAIT 1
#include <FastLED.h>
// ARGB config
#define FRAMETIME_US 1000000UL/75
#define FANS_OUT_PIN 9
#define FANS_LED_CT 9
CRGB leds_Fans[FANS_LED_CT];
#define BORDER_OUT_PIN 10
#define BORDER_LED_CT 94
CRGB leds_Border[BORDER_LED_CT];
#define BRACKET_OUT_PIN 6
#define BRACKET_LED_CT 6
CRGB leds_Bracket[BRACKET_LED_CT];
#define FILL_SMOOTHIE(leds, led_ct, offset) \
  for (uint8_t px = 0; px < (led_ct); ++px) (leds)[px].setHue(px*UINT8_MAX/(led_ct) + (offset))
int main(void) {
  FastLED.addLeds<NEOPIXEL, FANS_OUT_PIN>(leds_Fans, FANS_LED_CT);
  FastLED.addLeds<NEOPIXEL, BORDER_OUT_PIN>(leds_Border, BORDER_LED_CT);
  FastLED.addLeds<NEOPIXEL, BRACKET_OUT_PIN>(leds_Bracket, BRACKET_LED_CT);
  FastLED.setMaxRefreshRate(0);
  uint8_t offset = 0;
  blink_isr_enable();
  for (;;) {
    _delay_us(FRAMETIME_US);
    FILL_SMOOTHIE(leds_Fans, FANS_LED_CT, offset);
    FILL_SMOOTHIE(leds_Border, BORDER_LED_CT, offset);
    FILL_SMOOTHIE(leds_Bracket, BRACKET_LED_CT, offset);
    ++offset;
    FastLED.show();
  }
  return 0;
}
