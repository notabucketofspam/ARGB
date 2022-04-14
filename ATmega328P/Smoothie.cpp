#include "Smoothie.h"
void task_Smoothie(void *arg) {
  CRGB *leds;
  uint16_t led_ct;
  uint8_t controller_bit = 1;
  do {
    CLEDController *controller =  (CLEDController *)arg;
    leds = controller->leds();
    led_ct = controller->size();
    CLEDController *pCursor = controller->head();
    while (pCursor != controller) {
      controller_bit <<= 1;
      pCursor = pCursor->next();
    }
  } while (0);
  uint8_t offset = 0;
  for (;;) {
    xEventGroupSync(xControllerEGH, controller_bit, 0x80, portMAX_DELAY);
    // --- ARGB code goes here
    for (uint16_t px = 0; px < led_ct; ++px)
      leds[px].setHue((uint8_t)(px*UINT8_MAX/led_ct + (uint16_t)offset));
    ++offset;
  }
}
