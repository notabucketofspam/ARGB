#!/bin/sh
sed -i 1i"#include <string.h>" ./FreeRTOS/miniAVRfreeRTOS/hooks.c
sed -i s,"#warning \"Watchdog Timer used for scheduler.\"","//#warning \"Watchdog Timer used for scheduler.\"", \
./FreeRTOS/miniAVRfreeRTOS/port.c
sed -i 1i"#include <Arduino.h>" ./Adafruit_NeoPixel/Adafruit_NeoPixel/Adafruit_NeoPixel.h
sed -i 1i"#include <Arduino.h>" ./FastLED/FastLED/src/FastLED.h
