# STM32 with LM35 Temperature sensor

Here is simple project for [STM NUCLEO-F767ZI][STM NUCLEO-F767ZI] 
where I will:
* query [LM35][LM35] Sensor for temperature every second using ADC (LM35 outputs
  temperature in Celsius times 10 - for example 260 mV = 26.0 C
* print result on USART 3 which is directly connected to ST-LINK Virtual COM
  port

Problem:
- very significant jitter on ADC - even input connected
  to ground varies from minimum 0 to 78 or so which is error
  `100*78/4096 = 1.9%` (!)
- probably hit by errata:
  - https://www.st.com/resource/en/application_note/an4073-how-to-improve-adc-accuracy-when-using-stm32f2xx-and-stm32f4xx-microcontrollers-stmicroelectronics.pdf
  -  https://www.st.com/resource/en/errata_sheet/es0334-stm32f76xxx-and-stm32f77xxx-device-errata-stmicroelectronics.pdf
- will do more experiments (using median instead of average?)

Status:
- finished but problem with high jitter (see above)
- applied software workaround - averaging 32 samples.
- output on UART:
  ```
  L170: App v1.01
  L188: ADC Stats: last=278 avg=287 min=266 max=328 range=62 n=32
  L193: #1 ADC AVG(32) T=23.13 [^C] U=231.28 [mV] raw=287 (0x11f)
  L188: ADC Stats: last=274 avg=289 min=264 max=308 range=44 n=32
  L193: #2 ADC AVG(32) T=23.29 [^C] U=232.89 [mV] raw=289 (0x121)
  ...
  ```
  every second (where `Lx` is Line in main.c source, `#x`
  is simple measurement counter, `T` is temperature
  in Degrees of Celsius `U` is voltage in `mV`
- the STM32F7 USART3 is directly connected to ST-LINK Virtual Com port
  (no extra hardware needed). In my case it has
  name `STMicroelectronics STLink Virtual COM Port`. Use 115200 Baud, 8-bit,
  no parity, no flow control
- blinks green LED LD1 at 2s rate (toggle at 1s rate)
- Red LED LD3 on in case of fatal error

![Nucleo with LM35](assets/nucelo-lm35.jpg)

This project is companion
for [Measure temperature with PIC16F1579 and LM35 sensor](https://github.com/hpaluch/PIC16F1579-LM35-Temp).
I like to see how these very different MCUs an their tools compares.

# Setup

Required Hardware:
* [STM NUCLEO-F767ZI][STM NUCLEO-F767ZI] development board with
  STM32-F767ZI Cortex-M7 CPU at 216 MHz.
* [LM35][LM35] sensors TO-92 package that converts temperature
  to mili-volts times 10 ( 260 mV = 26.0 Celsius )

Wiring:

| Nucleo Board | LM 35 |
| --- | --- |
| CN8 PIN11 GND | PIN3 GND |
| CN8 PIN9 +5V  | PIN1 +Vs |
| CN10 PIN29 PA0 | PIN2 Vout through 470 Ohm resistor |

Above 470 Ohm resistor recommend for protection of both
LM35 and STM32F7.

Also recommended 100 nF capacitor as close as possible
to PIN1 and PIN3 of LM35.

Required Software:
* [STM32CubeMX][STM32CubeMX] tested version 6.9.2 `en.stm32cubemx-win-v6-9-2.zip`
* Under STM32CubeMX you have to install `STM32Cube_FW_F7_V1.17.1`,
  it should be right under path `c:\Ac6\STM32Cube\Repo` because
  this project has hardcoded dependencies on it.
* [STM32CubeIDE-Win](https://www.st.com/en/development-tools/stm32cubeide.html)
  tested version 1.13.2 `en.st-stm32cubeide_1.13.2_18220_20230914_1601_x86_64.exe.zip`

WARNING! CubeMX generator no longer supports 
"System Workbench for STM32"! You need to install
"STM32CubeIDE" to build this project.

# Build

TODO

# Troubleshooting

If your IDE will refuse to build project and reports something like:

> Orphaned configuration. No base extension cfg exists for
> com.st.stm32cube.ide.mcu.gnu.managedbuild.config.exe.debug.1868357600

It means that you are using wrong IDE - `SW4STM32` while
this project is for more recent `STM32CubeIDE`. To fix this
error download and install STM32CubeIDE
from https://www.st.com/en/development-tools/stm32cubeide.html

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

