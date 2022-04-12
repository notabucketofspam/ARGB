#!/bin/sh
cd FreeRTOS/miniAVRfreeRTOS && git apply ../../FreeRTOS.patch && cd ../..
cd Adafruit_NeoPixel/Adafruit_NeoPixel && git apply ../../Adafruit_NeoPixel.patch && cd ../..
cd FastLED/FastLED && git apply ../../FastLED.patch && cd ../..
