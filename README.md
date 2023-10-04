# STM32 with LM35 Temperature sensor

Here is planned simple project for [STM NUCLEO-F767ZI][STM NUCLEO-F767ZI] 
where I will:
* query LM35 Sensor for temperature every second using ADC (LM35 outputs
  temperature in Celsius times 10 - for example 260 mV = 26.0 C
* print result on USART 3 which is directly connected to ST-LINK Virtual COM
  port

Status:
- Does not work yet (setting up CubeMX generator so far)
- STM32SWT does not build project saying:

```
Orphaned configuration. No base extension cfg exists for
com.st.stm32cube.ide.mcu.gnu.managedbuild.config.exe.debug.1868357600
```

This project will be companion
for [Measure temperature with PIC16F1579 and LM35 sensor](https://github.com/hpaluch/PIC16F1579-LM35-Temp). I like to see how these very different MUs an their tools
compares.

# Setup

Required Hardware:
* [STM NUCLEO-F767ZI][STM NUCLEO-F767ZI] development board with Cortex-M7 CPU. 
  Ordered [STM NUCLEO-F767ZI - Amazon.de][STM NUCLEO-F767ZI - Amazon.de]

Required Software:
* [STM32CubeMX][STM32CubeMX] version 6.9.2
* Under STM32CubeMX you have to install `STM32Cube_FW_F7_V1.17.1`,
  it should be right under path `c:\Ac6\STM32Cube\Repo` because
  this project has hardcoded dependencies on it.
* [System Workbench for STM32][System Workbench for STM32] development IDE

# Build

TODO

# Resources

* Please see my [Getting started with ST NUCLEO F767ZI Board][Getting started with ST NUCLEO F767ZI Board]
  for introduction.

[STM32CubeF7]: https://www.st.com/en/embedded-software/stm32cubef7.html
[System Workbench for STM32]: http://www.openstm32.org/System%2BWorkbench%2Bfor%2BSTM32
[STM32CubeMX]: https://www.st.com/content/st_com/en/products/development-tools/software-development-tools/stm32-software-development-tools/stm32-configurators-and-code-generators/stm32cubemx.html

[STM NUCLEO-F767ZI]: https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-nucleo/nucleo-f767zi.html
[Getting started with ST NUCLEO F767ZI Board]: https://github.com/hpaluch/hpaluch.github.io/wiki/Getting-started-with-ST-NUCLEO-F767ZI-Board
[STM32CubeF7]: https://www.st.com/en/embedded-software/stm32cubef7.html

