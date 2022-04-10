#!/bin/sh
echo -en "#include <string.h>\\n$(<./FreeRTOS/miniAVRfreeRTOS/hooks.c)\\n"\
>./FreeRTOS/miniAVRfreeRTOS/hooks.c
sed -i s,"#warning \"Watchdog Timer used for scheduler.\"","//#warning \"Watchdog Timer used for scheduler.\"", \
./FreeRTOS/miniAVRfreeRTOS/port.c
echo -en "#include <Arduino.h>\\n$(<./FastLED/FastLED/src/FastLED.h)\\n"\
>./FastLED/FastLED/src/FastLED.h
echo -en "#include <Arduino.h>\\n$(<./Adafruit_NeoPixel/Adafruit_NeoPixel/Adafruit_NeoPixel.h)\\n"\
>./Adafruit_NeoPixel/Adafruit_NeoPixel/Adafruit_NeoPixel.h
