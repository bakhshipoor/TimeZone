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
#include "dma.h"
#include "dma2d.h"
#include "fatfs.h"
#include "i2c.h"
#include "ltdc.h"
#include "quadspi.h"
#include "rtc.h"
#include "sdmmc.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lvgl.h"
#include "tft.h"
#include "touchpad.h"
#include "stdio.h"
#include "stdlib.h"
#include "UI/time_zone_ui.h"
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
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
RTC_TimeTypeDef UTC_Time;
RTC_DateTypeDef UTC_Date;

static void scr_Main_Event(lv_event_t* e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if (event_code == LV_EVENT_DELETE)
    {
        lv_timer_delete(timer_update_data);
    }

}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	UTC_Time.Hours=12;
	UTC_Time.Minutes=0;
	UTC_Time.Seconds=0;
	UTC_Time.SubSeconds=0;
	UTC_Time.SecondFraction=1000;
	UTC_Time.TimeFormat=RTC_HOURFORMAT12_PM;
	UTC_Time.DayLightSaving=RTC_DAYLIGHTSAVING_NONE;
	UTC_Time.StoreOperation=RTC_STOREOPERATION_RESET;
	UTC_Date.Year=25;
	UTC_Date.Month=1;
	UTC_Date.Date=1;
	UTC_Date.WeekDay=RTC_WEEKDAY_WEDNESDAY;
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
  MX_DMA2D_Init();
  MX_I2C3_Init();
  MX_LTDC_Init();
  MX_RTC_Init();
  MX_FMC_Init();
  MX_QUADSPI_Init();
  MX_SDMMC1_SD_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */

  UTC_Time.Hours = 12;
  UTC_Time.Minutes = 0;
  UTC_Time.Seconds = 0;
  HAL_RTC_SetTime(&hrtc, &UTC_Time, RTC_FORMAT_BIN);

  UTC_Date.Year = 25;
  UTC_Date.Month = 1;
  UTC_Date.Date = 29;
  UTC_Date.WeekDay = RTC_WEEKDAY_WEDNESDAY;
  HAL_RTC_SetDate(&hrtc, &UTC_Date, RTC_FORMAT_BIN);

  HAL_RTC_GetTime(&hrtc, &UTC_Time, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc, &UTC_Date, RTC_FORMAT_BIN);

  Year = UTC_Date.Year+2000;
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
  lv_obj_set_style_bg_color(scr_Main, lv_color_hex(0xFFFFFF),0);
  lv_obj_set_style_text_color(scr_Main, lv_color_hex(0x000000),0);
  lv_obj_add_event_cb(scr_Main, scr_Main_Event, LV_EVENT_ALL, NULL);
  lv_screen_load(scr_Main);

  tz_ui();
  uint32_t counter=0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  FRESULT res;
  UINT bytesWritten=0;
  UINT bytesRead=0;
  // Mount SD Card (Drive 1)

  res = f_mount(&SDFatFS, SDPath, 1);
  if (res != FR_OK) {
	  Error_Handler();
  }
  char sd_file_path[20];
  snprintf(sd_file_path, sizeof(sd_file_path), "%ssd_test.txt", SDPath);
  res = f_open(&SDFile, sd_file_path, FA_CREATE_ALWAYS | FA_WRITE);
  if (res == FR_OK) {
	  f_write(&SDFile, "Hello from SD Card!", strlen("Hello from SD Card!"), &bytesWritten);
	  f_close(&SDFile);
  }
  f_mount(NULL, SDPath, 1);


  FRESULT res3=0;
  FRESULT res1 = f_mount(&USERFatFS, USERPath, 1);

  //BYTE workBuffer[4096];
  //FRESULT res2 = f_mkfs(USERPath, FM_FAT, 512, workBuffer, sizeof(workBuffer));

	char qspi_file_path[20];
	char read_buf[100];
	const char write_buf[] = "Hello from QUADSPI Memory. Lucky!";
	snprintf(qspi_file_path, sizeof(qspi_file_path), "%sqspi_test.txt", USERPath);
	res = f_open(&USERFile, qspi_file_path, FA_CREATE_ALWAYS | FA_WRITE);
	if (res == FR_OK) {
		res3 = f_write(&USERFile, write_buf, strlen(write_buf), &bytesWritten);
	  f_close(&USERFile);
	}

	memset(read_buf, 0, sizeof(read_buf));
	res = f_open(&USERFile, qspi_file_path, FA_READ );
	if (res == FR_OK) {
		res3 = f_read(&USERFile, read_buf, strlen(read_buf)-1, &bytesRead);
	  f_close(&USERFile);
	}

	f_mount(NULL, USERPath, 1);

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  HAL_Delay(5);
	  lv_task_handler();
	  counter++;
	  if (counter>20)
	  {
		  HAL_RTC_GetTime(&hrtc, &UTC_Time, RTC_FORMAT_BIN);
		  HAL_RTC_GetDate(&hrtc, &UTC_Date, RTC_FORMAT_BIN);
		  Year = UTC_Date.Year+2000;
		  Month = UTC_Date.Month;
		  Day = UTC_Date.Date;
		  Hours = UTC_Time.Hours;
		  Minutes = UTC_Time.Minutes;
		  Seconds = UTC_Time.Seconds;
		  counter=0;
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
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
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
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC|RCC_PERIPHCLK_SDMMC1
                              |RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 384;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 2;
  PeriphClkInitStruct.PLLSAI.PLLSAIQ = 2;
  PeriphClkInitStruct.PLLSAI.PLLSAIP = RCC_PLLSAIP_DIV8;
  PeriphClkInitStruct.PLLSAIDivQ = 1;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_8;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLLSAIP;
  PeriphClkInitStruct.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_CLK48;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

 /* MPU Configuration */

void MPU_Config(void)
{

  /* Disables the MPU */
  HAL_MPU_Disable();
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
