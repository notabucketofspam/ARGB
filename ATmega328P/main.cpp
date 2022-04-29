#include <FreeRTOS.h>
#include <task.h>
#include <event_groups.h>
// micros() in ArduinoCore wiring.c breaks with FreeRTOS, so disable fastled_delay.h
#define NO_MINIMUM_WAIT 1
#include <FastLED.h>
#include "Blink.h"
#include "Smoothie.h"
#define SET_BIT_RANGE(msb, lsb) ((((1<<(msb))-1)|(1<<(msb)))^((1<<(lsb))-1))
EventGroupHandle_t xControllerEGH;
void task_Show(void *arg);
// ARGB config
#define RADIANTD_OUT_PIN 9
#define RADIANTD_LED_CT 9
CRGB leds_RadiantD[RADIANTD_LED_CT];
#define BORDER_OUT_PIN 10
#define BORDER_LED_CT 94
CRGB leds_Border[BORDER_LED_CT];
#define BRACKET_OUT_PIN 11
#define BRACKET_LED_CT 6
CRGB leds_Bracket[BRACKET_LED_CT];
int main(void) {
  xControllerEGH = xEventGroupCreate();
  xTaskCreate(task_Blink, "Blink", 64, NULL, 1, NULL);
  xTaskCreate(task_Show, "Show", 64, NULL, 2, NULL);
  // Add controllers and tasks
  CLEDController *controller_RadiantD =
    &FastLED.addLeds<NEOPIXEL, RADIANTD_OUT_PIN>(leds_RadiantD, RADIANTD_LED_CT);
  xTaskCreate(task_Smoothie, "RadiantD", 64, controller_RadiantD, 1, NULL);
  CLEDController *controller_Border =
    &FastLED.addLeds<NEOPIXEL, BORDER_OUT_PIN>(leds_Border, BORDER_LED_CT);
  xTaskCreate(task_Smoothie, "Border", 64, controller_Border, 1, NULL);
  CLEDController *controller_Bracket =
    &FastLED.addLeds<NEOPIXEL, BRACKET_OUT_PIN>(leds_Bracket, BRACKET_LED_CT);
  xTaskCreate(task_Smoothie, "Bracket", 64, controller_Bracket, 1, NULL);
  // Like above, this will remove a check against micros() in FastLED.cpp
  FastLED.setMaxRefreshRate(0);
  vTaskStartScheduler();
  return 0;
}
void task_Show(void *arg) {
  // Frame time is ~15ms, as FreeRTOS uses the watchdog timer
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const EventBits_t bits_AllTasks = SET_BIT_RANGE(FastLED.count() - 1, 0);
  for (;;) {
    xTaskDelayUntil(&xLastWakeTime, 1);
    xEventGroupSync(xControllerEGH, 0x80, bits_AllTasks, portMAX_DELAY);
    xEventGroupClearBits(xControllerEGH, 0x80|bits_AllTasks);
    taskENTER_CRITICAL();
    FastLED.show();
    taskEXIT_CRITICAL();
  }
}
