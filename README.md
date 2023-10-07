# STM32 with LM35 Temperature sensor

Here is simple project for [STM NUCLEO-F767ZI][STM NUCLEO-F767ZI] Development
board where we use [LM35][LM35] sensor to measure temperature in Degrees of
Celsius and print it on UART.

![Nucleo with LM35](assets/nucelo-lm35.jpg)

Example UART output:

```
L170: App v1.02
L188: ADC Stats: last=307 avg=308 min=305 max=314 range=9 n=32
L193: #1 ADC AVG(32) T=24.82 [^C] U=248.21 [mV] raw=308 (0x134)
L188: ADC Stats: last=311 avg=309 min=304 max=314 range=10 n=32
L193: #2 ADC AVG(32) T=24.90 [^C] U=249.01 [mV] raw=309 (0x135)
L188: ADC Stats: last=307 avg=309 min=302 max=316 range=14 n=32
L193: #3 ADC AVG(32) T=24.90 [^C] U=249.01 [mV] raw=309 (0x135)
L188: ADC Stats: last=313 avg=309 min=304 max=316 range=12 n=32
L193: #4 ADC AVG(32) T=24.90 [^C] U=249.01 [mV] raw=309 (0x135)
...
```

Printed every second where `Lx` is Line in [Core/Src/main.c][Core/Src/main.c]
source, `#x` is simple measurement counter (increments by 1 on every
measurement), `T` is temperature in Degrees of Celsius `U` is voltage in `mV`

How it works:

1. [LM35][LM35] sensor converts temperature in Degrees of Celsius to mili-volts times 10,
   for example:
   - 10.5 °C is converted to 105 mV
   - 26.0 °C is converted to 260 mV
   - LIMITATION: to measure negative temperature there is required negative bias.
     It is not supported on this project, because it requires higher voltage
     than available +5V
2. STM32F7 uses 12-bit ADC1, input IN9 to convert mili-volts to unsigned integer.
3. ADC range is 0 to 4095 (12-bit) which corresponds to range 0 mV to 3300 mV
   So we have to transform it.
4. Also original ADC has high jitter so we measure input 32 times and computer average
3. Regarding ADC finally we have to convert mili-volts to Celsius by dividing them by 10
4. Finally we print measured temperature using regular `printf()` function on UART (USART3)
   output.
5. Because USART3 is directly connected to Virtual COM port of ST-LINK Programmer/Debugger
   users can just use Putty or any other terminal program to see USART3 output on PC

Resolved problems:
- had very significant jitter on ADC - even input connected
  to ground varies from minimum 0 to 78 or so which is error `100*78/4096 = 1.9%` (!)
- jitter reduced by using rather:
  - AGND CN10 PIN3 (Analog ground) instead of GND
  - ADC12_IN9 CN10 PIN 7 - ADC1 input IN9 - these pins are reserved for ADC
  - jitter reduced from 78 to 8 (0.2% instead of 1.9%)
- not sure how much of jitter is caused by this errata:
  - https://www.st.com/resource/en/application_note/an4073-how-to-improve-adc-accuracy-when-using-stm32f2xx-and-stm32f4xx-microcontrollers-stmicroelectronics.pdf
  -  https://www.st.com/resource/en/errata_sheet/es0334-stm32f76xxx-and-stm32f77xxx-device-errata-stmicroelectronics.pdf

Additional functions:
- blinks green LED LD1 at 2s rate (toggle at 1s rate)
- Red LED LD3 on in case of fatal error


This project is companion
for [Measure temperature with PIC16F1579 and LM35 sensor](https://github.com/hpaluch/PIC16F1579-LM35-Temp).
I like to see how these very different MCUs an their tools compares.

# Setup

Required Hardware:
* [STM NUCLEO-F767ZI][STM NUCLEO-F767ZI] development board with
  STM32-F767ZI Cortex-M7 CPU at 216 MHz.
* [LM35][LM35] sensor int TO-92 package that converts temperature
  to mili-volts times 10 ( 260 mV = 26.0 Celsius )

Wiring:

| Nucleo Board | LM 35 |
| --- | --- |
| CN10 PIN3 AGND (Analog Ground) | PIN3 GND |
| CN8 PIN9 +5V  | PIN1 +Vs |
| CN10 PIN7 PB1 IN9 | PIN2 Vout through 470 Ohm resistor |

Above 470 Ohm resistor recommend for protection of both
LM35 and STM32F7.

Also recommended 100 nF capacitor as close as possible
to PIN1 and PIN3 of LM35.

Required Software:
* [STM32CubeIDE-Win](https://www.st.com/en/development-tools/stm32cubeide.html)
  tested version 1.13.2 `en.st-stm32cubeide_1.13.2_18220_20230914_1601_x86_64.exe.zip`

If you plan to change any Hardware or HAL component you should also install:
* [STM32CubeMX][STM32CubeMX] tested version 6.9.2 `en.stm32cubemx-win-v6-9-2.zip`
* Under STM32CubeMX you have to install package: `STM32Cube_FW_F7_V1.17.1`,

WARNING! CubeMX generator no longer supports 
"System Workbench for STM32"! You need to install
"STM32CubeIDE" to build this project.

# Build

In STM32CubeIDE just click on Hammer icon. At the end of build you should
see on console messages like:

```
   text	   data	    bss	    dec	    hex	filename
  26788	    476	   2144	  29408	   72e0	stm32-lm35-temp.elf
Finished building: default.size.stdout

09:52:03 Build Finished. 0 errors, 0 warnings. (took 933ms)
```

# Running

In STM32CubeIDE do this:
* on toolbar just click on Green Arrow with tooltip `Run stm32-lm35-temp`
* when finished you should see in Console Window messages like:

```
ST-LINK SN  : [REDACTED]
ST-LINK FW  : V2J42M27
Board       : NUCLEO-F767ZI
Voltage     : 3.26V
SWD freq    : 4000 KHz
Connect mode: Under Reset
Reset mode  : Hardware reset
Device ID   : 0x451
Revision ID : Rev Z
Device name : STM32F76x/STM32F77x
Flash size  : 2 MBytes
Device type : MCU
Device CPU  : Cortex-M7
BL Version  : 0x93

Memory Programming ...
Opening and parsing file: ST-LINK_GDB_server_a01856.srec
  File          : ST-LINK_GDB_server_a01856.srec
  Size          : 26.63 KB
  Address       : 0x08000000

Erasing memory corresponding to segment 0:
Erasing internal memory sector 0
Download in Progress:

File download complete
Time elapsed during download operation: 00:00:00.977

Verifying ...
Download verified successfully

Shutting down...
Exit.
```

Since then the program should be running:
- green LED LD1 slowly blinking
- temperature output on UART every second

# Troubleshooting

If your IDE will refuse to build project and reports something like:

> Orphaned configuration. No base extension cfg exists for
> com.st.stm32cube.ide.mcu.gnu.managedbuild.config.exe.debug.1868357600

It means that you are using wrong IDE - `SW4STM32` while
this project is for more recent `STM32CubeIDE`. To fix this
error download and install STM32CubeIDE
from https://www.st.com/en/development-tools/stm32cubeide.html

Reliability problem:
* it happened once that when I connected and disconnected ST-LINK from USB several times the
  program stopped working - no blinking LED and no output on UART (but also no red LED indicating
  fatal error)
* the only remedy was to reprogram CPU again
* the cause of this problem is unknown

# Resources

* Please see my [Getting started with ST NUCLEO F767ZI Board][Getting started with ST NUCLEO F767ZI Board]
  for introduction.

[LM35]: https://www.ti.com/lit/ds/symlink/lm35.pdf
[STM32CubeIDE-Win]: https://www.st.com/en/development-tools/stm32cubeide.html
[STM32CubeF7]: https://www.st.com/en/embedded-software/stm32cubef7.html
[System Workbench for STM32]: http://www.openstm32.org/System%2BWorkbench%2Bfor%2BSTM32
[STM32CubeMX]: https://www.st.com/content/st_com/en/products/development-tools/software-development-tools/stm32-software-development-tools/stm32-configurators-and-code-generators/stm32cubemx.html
[STM NUCLEO-F767ZI]: https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-nucleo/nucleo-f767zi.html
[Getting started with ST NUCLEO F767ZI Board]: https://github.com/hpaluch/hpaluch.github.io/wiki/Getting-started-with-ST-NUCLEO-F767ZI-Board

