/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h> // printf(3)
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef uint16_t t_adc_sample;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
struct s_adc_stats {
	t_adc_sample minVal;
	t_adc_sample maxVal;
	t_adc_sample lastVal;
	t_adc_sample avgVal;
	int n; // number of processed samples
};
#define APP_ADC_STATS_INIT {0,0,0,0,0}
#define N_AVG_SAMPLES 16 // maximum for uint16_t sum
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define APP_VERSION 101 // 123 = 1.23
#define APP_ADC_VREF_PLUS_FP 3300.0 // 3300 mV for 4095 value from ADC
#define APP_ADC_RANGE 4096
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
unsigned int gCounter=0; // g as "global"
bool gUartStarted=false; // if Error_Handler() can use USART3 to report error
// remember that STM32F7 has hardware FPU! - so no reason to stick with integers!
double gAdcMiliV=0;
struct s_adc_stats gAdcStats = APP_ADC_STATS_INIT;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
// redirecting printf(3) to USART3, from
// c:\Ac6\STM32Cube_FW_F7_V1.17.0\Projects\STM32F767ZI-Nucleo\Examples\UART\UART_Printf\Src\main.c
#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// returns single 12-bit sample (0 to 4095: equivalent of 0V to 3.3V) from ADC1 PA0/IN0 (CN10 PIN29)
t_adc_sample GetSingleAdcSample(void)
{
	  /* Start the conversion process */
	  if (HAL_ADC_Start(&hadc1) != HAL_OK)
	  {
	    /* Start Conversation Error */
		printf("ERROR: L%d: HAL_ADC_Start() failed.\r\n", __LINE__);
		Error_Handler();
	  }
	  /*  For simplicity reasons, this example is just waiting till the end of the
	      conversion, but application may perform other tasks while conversion
	      operation is ongoing. */
	  if (HAL_ADC_PollForConversion(&hadc1, 10) != HAL_OK){
	    /* End Of Conversion flag not set on time */
		printf("ERROR: L%d: HAL_ADC_PollForConversion() failed.\r\n", __LINE__);
	    Error_Handler();
	  }
	  /* ADC conversion completed */
	  return HAL_ADC_GetValue(&hadc1);
}

// we must average several samples because ADC output is impacted by significant
// noise from Flash and other circuits:
// - es0334-stm32f76xxx-and-stm32f77xxx-device-errata-stmicroelectronics.pdf
// - an4073-how-to-improve-adc-accuracy-when-using-stm32f2xx-and-stm32f4xx-microcontrollers-stmicroelectronics.pdf
struct s_adc_stats GetAveragedAdcSamples(int n)
{
	int i;
	uint32_t adcSum=0; // must use 32-bit type for n > 16 to avoid overflow!!!
	struct s_adc_stats s = APP_ADC_STATS_INIT;

	if (n<0 || n>=65535){
		printf("ERROR: L%d Specified number of samples n=%d out of allowed range!\r\n",
			__LINE__, n);
		Error_Handler();
	}
	s.n = n;
	for(i=0;i<n;i++){
		s.lastVal = GetSingleAdcSample();
		if (i==0){
			s.minVal = s.lastVal;
			s.maxVal = s.lastVal;
		} else {
			if (s.minVal > s.lastVal) s.minVal = s.lastVal;
			if (s.maxVal < s.lastVal) s.maxVal = s.lastVal;
		}
		adcSum += s.lastVal;
	}
	s.avgVal = adcSum/n;
	return s;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART3_UART_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  gUartStarted=true; // tell Error_Handler() that it can print fatal error to USART3
  printf("L%d: App v%d.%02d\r\n", __LINE__, APP_VERSION/100, APP_VERSION%100);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  gCounter++;
	  // read ADC1, on PA0/AN0
	  // we must average data to workaround ADC noise errata:
	  // see: es0334-stm32f76xxx-and-stm32f77xxx-device-errata-stmicroelectronics.pdf
	  // - section 2.2.1 Internal noise impacting the ADC accuracy
	  // and whole: an4073-how-to-improve-adc-accuracy-when-using-stm32f2xx-and-stm32f4xx-microcontrollers-stmicroelectronics.pdf
	  gAdcStats = GetAveragedAdcSamples(32);
	  printf("L%d: ADC Stats: last=%u avg=%u min=%u max=%u range=%d n=%d\r\n",
			  __LINE__,gAdcStats.lastVal, gAdcStats.avgVal, gAdcStats.minVal, gAdcStats.maxVal,
			  (int)gAdcStats.maxVal-(int)gAdcStats.minVal,gAdcStats.n);
	  // ADC Value is 12 bit (4096 values) 0=0V, 4095=3.3V (Vdd) on PA0/AN0
	  gAdcMiliV = gAdcStats.avgVal * APP_ADC_VREF_PLUS_FP / (APP_ADC_RANGE-1);
	  printf("L%d: #%u ADC AVG(%d) T=%.2f [^C] U=%.2f [mV] raw=%u (0x%x)\r\n",
			  __LINE__, gCounter, gAdcStats.n, gAdcMiliV/10.0, gAdcMiliV, gAdcStats.avgVal, gAdcStats.avgVal);
	  HAL_Delay(1000); // pause for 1s
	  HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 432;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
// redirecting printf(3) to USART3, from
// c:\Ac6\STM32Cube_FW_F7_V1.17.0\Projects\STM32F767ZI-Nucleo\Examples\UART\UART_Printf\Src\main.c
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART3 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  // red LED LD3 On forever ...
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
  if (gUartStarted){
	  printf("ERROR: L%d: SYSTEM HALTED due fatal error!\r\n", __LINE__);
  }
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
