/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "crc.h"
#include "dcmi.h"
#include "dma.h"
#include "dma2d.h"
#include "eth.h"
#include "fatfs.h"
#include "i2c.h"
#include "ltdc.h"
#include "quadspi.h"
#include "rtc.h"
#include "sai.h"
#include "sdmmc.h"
#include "spdifrx.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lvgl.h"
#include "tft.h"
#include "touchpad.h"
#include "stdio.h"
#include "stdlib.h"
#include "time_zone_ui.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
static void MPU_Config(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
RTC_TimeTypeDef UTC_Time;
RTC_DateTypeDef UTC_Date;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MPU Configuration--------------------------------------------------------*/
	MPU_Config();

	/* Enable the CPU Cache */

	/* Enable I-Cache---------------------------------------------------------*/
	SCB_EnableICache();

	/* Enable D-Cache---------------------------------------------------------*/
	SCB_EnableDCache();

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* Configure the peripherals common clocks */
	PeriphCommonClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_ADC3_Init();
	MX_CRC_Init();
	MX_DCMI_Init();
	MX_DMA2D_Init();
	MX_ETH_Init();
	MX_FMC_Init();
	MX_I2C1_Init();
	MX_I2C3_Init();
	MX_LTDC_Init();
	MX_QUADSPI_Init();
	MX_RTC_Init();
	MX_SAI2_Init();
	MX_SPDIFRX_Init();
	MX_SPI2_Init();
	MX_TIM1_Init();
	MX_TIM2_Init();
	MX_TIM3_Init();
	MX_TIM5_Init();
	MX_TIM8_Init();
	MX_TIM12_Init();
	MX_USART1_UART_Init();
	MX_USART6_UART_Init();
	MX_USB_DEVICE_Init();
	MX_SDMMC1_SD_Init();
	MX_FATFS_Init();

	/* Initialize interrupts */
	MX_NVIC_Init();
	/* USER CODE BEGIN 2 */
	  //FRESULT res = f_mount(&SDFatFS, SDPath, 1);
	  //if (res != FR_OK) {
	  //	Error_Handler();
	  //}

	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0xF2F2); // backup register

	UTC_Time.Hours = 12;
	UTC_Time.Minutes = 0;
	UTC_Time.Seconds = 0;
	UTC_Time.SubSeconds = 0;
	UTC_Time.SecondFraction = 1000;
	UTC_Time.TimeFormat = RTC_HOURFORMAT12_PM;
	UTC_Time.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	UTC_Time.StoreOperation = RTC_STOREOPERATION_RESET;
	UTC_Date.Year = 25;
	UTC_Date.Month = 1;
	UTC_Date.Date = 29;
	UTC_Date.WeekDay = RTC_WEEKDAY_WEDNESDAY;

	HAL_RTC_SetTime(&hrtc, &UTC_Time, RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&hrtc, &UTC_Date, RTC_FORMAT_BIN);

	HAL_RTC_GetTime(&hrtc, &UTC_Time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &UTC_Date, RTC_FORMAT_BIN);

	Year = UTC_Date.Year + 2000;
	Month = UTC_Date.Month;
	Day = UTC_Date.Date;

	Hours = UTC_Time.Hours;
	Minutes = UTC_Time.Minutes;
	Seconds = UTC_Time.Seconds;

	TZ_Init = tz_init();

	lv_init();
	tft_init();
	touchpad_init();

	lv_obj_t* scr_Main;
	scr_Main = lv_obj_create(NULL);
	lv_obj_set_style_bg_color(scr_Main, lv_color_hex(0xFFFFFF), 0);
	lv_obj_set_style_text_color(scr_Main, lv_color_hex(0x000000), 0);
	lv_screen_load(scr_Main);

	tz_ui();
	uint32_t counter = 0;
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	uint8_t is_init = 0;
	while (1)
	{
		HAL_Delay(5);
		lv_task_handler();
		if (BSP_PlatformIsDetected() == SD_PRESENT && is_init == 0)
		{
			// Card Inserted
			HAL_SD_Init(&hsd1);
			HAL_Delay(10);
			MX_USB_DEVICE_Start();
			HAL_Delay(10);
			is_init = 1;
		}
		else if (BSP_PlatformIsDetected() == SD_NOT_PRESENT && is_init == 1)
		{
			// Card Removed
			MX_USB_DEVICE_Stop();
			HAL_Delay(10);
			HAL_SD_DeInit(&hsd1);
			HAL_Delay(10);
			is_init = 0;
		}
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		counter++;
		if (counter > 40)
		{
			HAL_RTC_GetTime(&hrtc, &UTC_Time, RTC_FORMAT_BIN);
			HAL_RTC_GetDate(&hrtc, &UTC_Date, RTC_FORMAT_BIN);
			Year = UTC_Date.Year + 2000;
			Month = UTC_Date.Month;
			Day = UTC_Date.Date;
			Hours = UTC_Time.Hours;
			Minutes = UTC_Time.Minutes;
			Seconds = UTC_Time.Seconds;
			update_data();
			counter = 0;
		}
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enables the Clock Security System
  */
  HAL_RCC_EnableCSS();
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC|RCC_PERIPHCLK_SAI2
                              |RCC_PERIPHCLK_SDMMC1|RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 384;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 5;
  PeriphClkInitStruct.PLLSAI.PLLSAIQ = 2;
  PeriphClkInitStruct.PLLSAI.PLLSAIP = RCC_PLLSAIP_DIV8;
  PeriphClkInitStruct.PLLSAIDivQ = 1;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_8;
  PeriphClkInitStruct.Sai2ClockSelection = RCC_SAI2CLKSOURCE_PLLSAI;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLLSAIP;
  PeriphClkInitStruct.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_CLK48;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* OTG_FS_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(OTG_FS_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

	
}
/* USER CODE END 4 */

 /* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x0;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */
  if (htim->Instance == TIM6) {
   lv_tick_inc(1);
  }
  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
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
