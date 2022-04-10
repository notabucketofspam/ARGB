#ifndef SMOOTHIE_H
#define SMOOTHIE_H
#include <FreeRTOS.h>
#include <task.h>
#include <event_groups.h>
#include <FastLED.h>
extern EventGroupHandle_t xControllerEGH;
void task_Smoothie(void *arg);
#endif /* SMOOTHIE_H */
