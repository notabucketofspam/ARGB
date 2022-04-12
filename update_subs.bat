@echo off
cd FreeRTOS\miniAVRfreeRTOS && git checkout master && git pull && git submodule update && cd ..\..
cd ArduinoCore\ArduinoCore-avr && git checkout master && git pull && git submodule update && cd ..\..
cd Adafruit_NeoPixel\Adafruit_NeoPixel && git checkout master && git pull && git submodule update && cd ..\..
cd FastLED\FastLED && git checkout master && git pull && git submodule update && cd ..\..
