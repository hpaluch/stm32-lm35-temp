# STM32 with LM35 Temperature sensor

Here is planned simple project for [STM NUCLEO-F767ZI][STM NUCLEO-F767ZI] 
where I will:
* query LM35 Sensor for temperature every second using ADC (LM35 outputs
  temperature in Celsius times 10 - for example 260 mV = 26.0 C
* print result on USART 3 which is directly connected to ST-LINK Virtual COM
  port

Status:
- Just blink green LED LD1 at 2s rate (toggle at 1s rate)
- Red LED LD3 on in case of fatal error

This project will be companion
for [Measure temperature with PIC16F1579 and LM35 sensor](https://github.com/hpaluch/PIC16F1579-LM35-Temp). I like to see how these very different MUs an their tools
compares.

# Setup

Required Hardware:
* [STM NUCLEO-F767ZI][STM NUCLEO-F767ZI] development board with
  STM32-F767ZI Cortex-M7 CPU at 216 MHz.

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

It measn that you are using wrong IDE - `SW4STM32` while
this project is for more recent `STM32CubeIDE`. To fix this
error download and install STM32CubeIDE
from https://www.st.com/en/development-tools/stm32cubeide.html

# Resources

* Please see my [Getting started with ST NUCLEO F767ZI Board][Getting started with ST NUCLEO F767ZI Board]
  for introduction.

[STM32CubeIDE-Win]: https://www.st.com/en/development-tools/stm32cubeide.html
[STM32CubeF7]: https://www.st.com/en/embedded-software/stm32cubef7.html
[System Workbench for STM32]: http://www.openstm32.org/System%2BWorkbench%2Bfor%2BSTM32
[STM32CubeMX]: https://www.st.com/content/st_com/en/products/development-tools/software-development-tools/stm32-software-development-tools/stm32-configurators-and-code-generators/stm32cubemx.html

[STM NUCLEO-F767ZI]: https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-nucleo/nucleo-f767zi.html
[Getting started with ST NUCLEO F767ZI Board]: https://github.com/hpaluch/hpaluch.github.io/wiki/Getting-started-with-ST-NUCLEO-F767ZI-Board

