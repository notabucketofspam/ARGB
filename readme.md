# ARGB

Intended for use with an ATmega328P device, such as the Gelid Solutions CODI6, available [here][-1]. A list of complimentary ARGB products is [here][-2].

The FreeRTOS version of the project is now deprecated due to its lack of granular task scheduling.

Notes:

- The script files must be run first.
  - `patch.bat` will add some missing `#include` statements to the relevant files.
  - `update_subs.bat` will pull the latest changes for each submodule.
  - Unix shell scripts are provided as an alternative.
- Atmel Studio (now Microchip Studio) version 7.0.2542 is [here][0].
- Atmel ATmega Device Pack version 2.0.401 is [here][1].
  - Install it in Atmel Studio with `Tools (Alt + T) -> Device Pack Manager -> Install -> Browse pack file`.
- AVR-GCC version 11.1.0 is [here][2].
  - Unzip it somewhere accessible (i.e. `C:\opt`).
  - Add AVR-GCC to Atmel Studio with `Tools (Alt + T) -> Options (O) -> Toolchain -> Package Configuration`.
    - Select `Toolchains: Atmel AVR 8-bit (C language) -> Add Flavour`.
    - Package Name: `avr-gcc-11.1.0-x64-windows`.
    - Package Base Path: `bin` folder in wherever AVR-GCC was unzipped (i.e. `C:\opt\avr-gcc-11.1.0-x64-windows\bin`).
    - Repeat for `Atmel AVR 8-bit (CPP language)`.
  - Add Make with `Tools (Alt + T) -> Options (O) -> Builder -> GNU make`.
    - Select `ShellUtils Packages: Custom`.
    - Path: leave default (i.e. `C:\Program Files (x86)\Atmel\Studio\7.0\shellutils`).
    - Path to make: `make.exe` within the `bin` folder (i.e. `C:\opt\avr-gcc-11.1.0-x64-windows\bin\make.exe`).
  - Add AVRDUDE with `Tools (Alt + T) -> External Tools (E) -> Add`
    - Title: `avrdude`
    - Command: `avrdude.exe` within the `bin` folder (i.e. `C:\opt\avr-gcc-11.1.0-x64-windows\bin\avrdude.exe`).
    - Arguments: `-carduino -patmega328p -b115200 -D -Uflash:w:"$(TargetName).hex":i -C"C:\opt\avr-gcc-11.1.0-x64-windows\bin\avrdude.conf -PCOM#"`
      - Adjust `-PCOM#` for the relevant `COM` port (i.e. `-PCOM4`).
      - Adjust `-C"C:\opt\avr-gcc-11.1.0-x64-windows\bin\avrdude.conf"` for the location of `avrdude.conf`.
    - Initial directory: `$(TargetDir)`
    - Tick the box next to `Use Output window`.
  - Build output will no longer show `Program Memory Usage` or `Data Memory Usage`. However, `avr-size.exe` will give the size in bytes of the `text`, `data`, and `bss` memory sections; information about AVR memory sections is [here][3].
  - Debugging via the Atmel Studio Simulator, with or without GDB, will no longer work. An alternative external simulator, AVRSimV2, is [here][4].
    - If use of the Atmel Studio Simulator is absolutely necessary, then `Toolchain Flavour` under `Project Properties (Alt + F7) -> Advanced` must be reverted to `Native` for each project in the dependency tree.

[-1]: https://gelidsolutions.com/thermal-solutions/codi6-argb-controller/
[0]: https://www.microchip.com/en-us/tools-resources/develop/microchip-studio
[1]: http://packs.download.atmel.com/
[2]: https://github.com/ZakKemble/avr-gcc-build/releases
[3]: https://www.nongnu.org/avr-libc/user-manual/mem_sections.html
[4]: https://git.rwth-aachen.de/jonas.broeckmann/avrsimv2
[-2]: ./parts_list.md
