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
// Even with controller, can only control 6 per row
#define CABLE_LED_CT 27
CRGB leds_Cable[CABLE_LED_CT];
#define GPU_OUT_PIN 5
#define GPU_LED_CT 19
CRGB leds_Gpu[GPU_LED_CT];
#define GPU_BAR_LED_CT 6
#define BLUE_GREEN 0x00AA55
#define FAN_SHIFT(x) (x)*0x100u/FAN_LED_CT/FAN_CT
DEFINE_GRADIENT_PALETTE(gradient1) {
0x00, 0x00, 0xAA, 0x55, // Blue-green
0x10, 0x00, 0xAA, 0x55, // Blue-green
0x20, 0xFF, 0x00, 0x00, // Red
0x30, 0xFF, 0x00, 0x00, // Red
0x40, 0x00, 0xAA, 0x55, // Blue-green
0x50, 0xFF, 0x00, 0x00, // Red
0x60, 0xFF, 0x00, 0x00, // Red
0x70, 0x00, 0xAA, 0x55, // Blue-green
0xFF, 0x00, 0xAA, 0x55, // Blue-green until the end
};
CRGBPalette16 palette1 = gradient1;
DEFINE_GRADIENT_PALETTE(gradient2) {
  0x00, 0x00, 0xAA, 0x55, // Blue-green
  0x10, 0x00, 0xAA, 0x55, // Blue-green
  0x20, 0xFF, 0x00, 0x00, // Red
  0x30, 0xFF, 0x00, 0x00, // Red
  0x40, 0x00, 0xAA, 0x55, // Blue-green
  0xFF, 0x00, 0xAA, 0x55, // Blue-green until the end
};
CRGBPalette16 palette2 = gradient2;
#define HEX_FAN FAN_SHIFT(6) + 0x40
DEFINE_GRADIENT_PALETTE(gradient3) {
  0x00, 0x00, 0xAA, 0x55, // Blue-green
  0x10, 0x00, 0xAA, 0x55, // Blue-green
  0x20, 0xFF, 0x00, 0x00, // Red
  0x30, 0xFF, 0x00, 0x00, // Red
  0x40, 0x00, 0xAA, 0x55, // Blue-green
  HEX_FAN, 0x00, 0xAA, 0x55, // Blue-green
  HEX_FAN+0x10, 0xFF, 0x00, 0x00, // Red
  HEX_FAN+0x20, 0xFF, 0x00, 0x00, // Red
  HEX_FAN+0x30, 0x00, 0xAA, 0x55, // Blue-green
  0xFF, 0x00, 0xAA, 0x55, // Blue-green until the end
};
CRGBPalette16 palette3 = gradient3;
DEFINE_GRADIENT_PALETTE(gradient4) {
  0x00, 0x00, 0xAA, 0x55, // Blue-green
  0x10, 0x00, 0xAA, 0x55, // Blue-green
  0x50, 0xFF, 0xFF, 0xFF, // White
  0x90, 0x00, 0xAA, 0x55, // Blue-green
  0xFF, 0x00, 0xAA, 0x55, // Blue-green until the end
};
CRGBPalette16 palette4 = gradient4;
#define FILL_SMOOTHIE(leds, start, led_ct, offset) \
  for (uint8_t px = 0; px < (led_ct); ++px) (leds)[px + (start)].setHue(px*0x100u/(led_ct) + (offset))
#define GROUP_FROM_PALETTE(leds, start, led_ct, offset, palette, group_led_ct) \
  for (uint8_t px = 0; px < (led_ct); ++px) (leds)[px + (start)] = \
    ColorFromPalette((palette), px*0x100u/(group_led_ct) + (offset))
int main(void) {
  FastLED.addLeds<NEOPIXEL, FANS_IN_OUT_PIN>(leds_FansIn, FAN_LED_CT*FAN_CT);
  FastLED.addLeds<NEOPIXEL, FANS_EX_OUT_PIN>(leds_FansEx, FAN_LED_CT*FAN_CT);
  FastLED.addLeds<NEOPIXEL, BORDER_OUT_PIN>(leds_Border, BORDER_LED_CT);
  FastLED.addLeds<NEOPIXEL, BRACKET_OUT_PIN>(leds_Bracket, BRACKET_LED_CT);
  FastLED.addLeds<NEOPIXEL, CABLE_OUT_PIN>(leds_Cable, CABLE_LED_CT);
  FastLED.addLeds<NEOPIXEL, GPU_OUT_PIN>(leds_Gpu, GPU_LED_CT);
  // Most of the GPU is too hard to work with
  fill_solid(&leds_Gpu[GPU_BAR_LED_CT], GPU_LED_CT - GPU_BAR_LED_CT, BLUE_GREEN);
  FastLED.setMaxRefreshRate(0);
  uint8_t offset = 0;
  blink_isr_enable();
  for (;;) {
    _delay_us(FRAMETIME_US);
    // Top fan
    // Left
    for (uint8_t px = 2; px < FAN_LED_CT/2; ++px)
      leds_FansIn[px] = ColorFromPalette(palette2, FAN_SHIFT(px) + offset);
    //Right
    leds_FansIn[0] = ColorFromPalette(palette2, offset);
    for (uint8_t px = FAN_LED_CT/2 + 2; px < FAN_LED_CT; ++px)
      leds_FansIn[px] = ColorFromPalette(palette2, FAN_SHIFT(px + 24) + offset);
    // Top / bottom
    leds_FansIn[1] = ColorFromPalette(palette2, FAN_SHIFT(1) +  offset);
    leds_FansIn[7] = ColorFromPalette(palette3, FAN_SHIFT(7) + offset);
    // Center fan
    // Since the center is in reverse (CCW instead of CW), shifts have to align
    // opposite of the fan adjacent (i.e. top -> bottom, left -> right), with +1 for timing
    // Left
    for (uint8_t px = 2; px < FAN_LED_CT/2; ++px)
      leds_FansIn[px + FAN_LED_CT] = ColorFromPalette(palette2, FAN_SHIFT(px + 20) - offset);
    // Right
    leds_FansIn[FAN_LED_CT] = ColorFromPalette(palette2, FAN_SHIFT(8) - offset);
    for (uint8_t px = FAN_LED_CT/2 + 2; px < FAN_LED_CT; ++px)
      leds_FansIn[px + FAN_LED_CT] = ColorFromPalette(palette2, FAN_SHIFT(px + 32) - offset);
    // Top / bottom
    leds_FansIn[FAN_LED_CT + 1] = ColorFromPalette(palette3, FAN_SHIFT(20) - offset);
    leds_FansIn[FAN_LED_CT + 7] = ColorFromPalette(palette3, FAN_SHIFT(2) - offset);
    // Bottom fan
    // Left
    for (uint8_t px = 2; px < FAN_LED_CT/2; ++px)
      leds_FansIn[px + 2*FAN_LED_CT] = ColorFromPalette(palette2, FAN_SHIFT(px + FAN_LED_CT + 1) + offset);
    // Right
    leds_FansIn[2*FAN_LED_CT] = ColorFromPalette(palette2, FAN_SHIFT(2*FAN_LED_CT) + offset);
    for (uint8_t px = FAN_LED_CT/2 + 2; px < FAN_LED_CT; ++px)
      leds_FansIn[px + 2*FAN_LED_CT] = ColorFromPalette(palette2, FAN_SHIFT(px - 2*FAN_LED_CT - 4) + offset);
    // Top / bottom
    leds_FansIn[2*FAN_LED_CT + 1] = ColorFromPalette(palette3, FAN_SHIFT(2*FAN_LED_CT + 1) + offset);
    leds_FansIn[2*FAN_LED_CT + 7] = ColorFromPalette(palette2, FAN_SHIFT(4*FAN_LED_CT + 7) + offset);
    // Everything else
    FILL_SMOOTHIE(leds_FansEx, 0, FAN_LED_CT, offset);
    FILL_SMOOTHIE(leds_FansEx, FAN_LED_CT, FAN_LED_CT, -offset);
    FILL_SMOOTHIE(leds_FansEx, 2*FAN_LED_CT, FAN_LED_CT, offset + 0x80);
    FILL_SMOOTHIE(leds_Border, 0, BORDER_LED_CT, offset);
    GROUP_FROM_PALETTE(leds_Bracket, 0, BRACKET_LED_CT, offset, palette4, BRACKET_LED_CT);
    GROUP_FROM_PALETTE(leds_Cable, 0, CABLE_LED_CT, -offset, palette1, CABLE_LED_CT + GPU_BAR_LED_CT);
    GROUP_FROM_PALETTE(leds_Gpu, 0, GPU_BAR_LED_CT, offset + 0xB0, palette1, CABLE_LED_CT + GPU_BAR_LED_CT);
    ++offset;
    FastLED.show();
  }
  return 0;
}
