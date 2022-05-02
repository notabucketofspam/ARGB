#!/bin/sh
echo FreeRTOS && cd FreeRTOS/miniAVRfreeRTOS && git checkout master && git pull && git submodule update
echo ArduinoCore && cd ../../ArduinoCore/ArduinoCore-avr && git checkout master && git pull && git submodule update
echo Adafruit_NeoPixel && cd ../../Adafruit_NeoPixel/Adafruit_NeoPixel && git checkout master && git pull && git submodule update
echo FastLED && cd ../../FastLED/FastLED && git checkout master && git pull && git submodule update
echo avr-libstdcpp && cd ../../avr-libstdcpp/avr-libstdcpp && git checkout master && git pull && git submodule update
echo Done
