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
#define FAN_LED_CT 12
#define FAN_CT 3
#define FANS_IN_OUT_PIN 10
CRGB leds_FansIn[FAN_LED_CT*FAN_CT];
#define FANS_EX_OUT_PIN 3
CRGB leds_FansEx[FAN_LED_CT*FAN_CT];
#define BORDER_OUT_PIN 11
#define BORDER_LED_CT 94
CRGB leds_Border[BORDER_LED_CT];
#define BRACKET_OUT_PIN 6
#define BRACKET_LED_CT 6
CRGB leds_Bracket[BRACKET_LED_CT];
#define CABLE_OUT_PIN 9
#define CABLE_LED_CT 162
CRGB leds_Cable[CABLE_LED_CT];
#define FILL_SMOOTHIE(leds, start, led_ct, offset) \
  for (uint8_t px = 0; px < (led_ct); ++px) (leds)[px + (start)].setHue(px*0x100u/(led_ct) + (offset))
int main(void) {
  FastLED.addLeds<NEOPIXEL, FANS_IN_OUT_PIN>(leds_FansIn, FAN_LED_CT*FAN_CT);
  FastLED.addLeds<NEOPIXEL, FANS_EX_OUT_PIN>(leds_FansEx, FAN_LED_CT*FAN_CT);
  FastLED.addLeds<NEOPIXEL, BORDER_OUT_PIN>(leds_Border, BORDER_LED_CT);
  FastLED.addLeds<NEOPIXEL, BRACKET_OUT_PIN>(leds_Bracket, BRACKET_LED_CT);
  FastLED.addLeds<NEOPIXEL, CABLE_OUT_PIN>(leds_Cable, CABLE_LED_CT);
  fill_solid(leds_Cable, CABLE_LED_CT, CRGB(0xFF7B00));
  FastLED.setMaxRefreshRate(0);
  uint8_t offset = 0;
  blink_isr_enable();
  for (;;) {
    _delay_us(FRAMETIME_US);
    FILL_SMOOTHIE(leds_FansIn, 0, FAN_LED_CT, offset);
    FILL_SMOOTHIE(leds_FansIn, FAN_LED_CT, FAN_LED_CT, -offset);
    FILL_SMOOTHIE(leds_FansIn, 2*FAN_LED_CT, FAN_LED_CT, offset + 0x80);
    FILL_SMOOTHIE(leds_FansEx, 0, FAN_LED_CT, offset);
    FILL_SMOOTHIE(leds_FansEx, FAN_LED_CT, FAN_LED_CT, -offset);
    FILL_SMOOTHIE(leds_FansEx, 2*FAN_LED_CT, FAN_LED_CT, offset + 0x80);
    FILL_SMOOTHIE(leds_Border, 0, BORDER_LED_CT, offset);
    FILL_SMOOTHIE(leds_Bracket, 0, BRACKET_LED_CT, offset);
    ++offset;
    FastLED.show();
  }
  return 0;
}
