@echo off
echo FreeRTOS && cd FreeRTOS\miniAVRfreeRTOS && git apply ..\..\FreeRTOS.patch
echo Adafruit_NeoPixel && cd ..\..\Adafruit_NeoPixel\Adafruit_NeoPixel && git apply ..\..\Adafruit_NeoPixel.patch
echo FastLED && cd ..\..\FastLED\FastLED && git apply ..\..\FastLED.patch
echo Done && timeout 2 > nul
