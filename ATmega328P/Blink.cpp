// refer to avrfreertos/UnoBlink/main.c
#include "Blink.h"
void task_Blink(void *arg) {
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xPeriod = pdMS_TO_TICKS(500);
  DDRB |= _BV(DDB5);
  for (;;) {
    PORTB |= _BV(PORTB5);
    vTaskDelayUntil(&xLastWakeTime, xPeriod);
    PORTB &= ~_BV(PORTB5);
    vTaskDelayUntil(&xLastWakeTime, xPeriod);
  }
}
