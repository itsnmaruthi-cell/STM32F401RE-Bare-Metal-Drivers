/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include <stdint.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//*****USING LD2
volatile uint32_t* RCC_AHB1ENR=(volatile uint32_t*)0X40023830;
volatile uint32_t* GPIOA_MODER=(volatile uint32_t*)0X40020000;
volatile uint32_t* GPIOA_BSRR=(volatile uint32_t*)0X40020018;
//*******USING TIM2
volatile uint32_t* RCC_APB1ENR=(volatile uint32_t*)0X40023840;
//NEED POINTERS FOR FURTHER CONFIGURATION OF TIM2
volatile uint32_t* TIM2_CR1=(volatile uint32_t*)0X40000000;
volatile uint32_t* TIM2_DIER=(volatile uint32_t*)0X4000000C;
volatile uint32_t* TIM2_SR=(volatile uint32_t*)0X40000010;
volatile uint32_t* TIM2_PSC=(volatile uint32_t*)0X40000028;
volatile uint32_t* TIM2_ARR=(volatile uint32_t*)0X4000002C;
//NEED NVIC TO TELL CPU TO PROCESS INTERRUPTS FROM TIM2
//IRQ NUMBER IS 28 WHICH IS IN ISER0 RANGE
volatile uint32_t* NVIC_ISER0=(volatile uint32_t*)0XE000E100;
void TIM2_IRQHandler(void);
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
  /* USER CODE BEGIN 2 */
  *RCC_AHB1ENR|=(1U<<0);//GPIOA ENABLE
  *RCC_APB1ENR|=(1U<<0);//TIM2 CLOCK ENABLE
  *GPIOA_MODER&=~(3U<<10);
  *GPIOA_MODER|=(1U<<10);//PA5 OUTPUT
  *NVIC_ISER0|=(1U<<28);//TELLING PROCESSOR TO MANAGE INTERRUPTS FROM TIM2
  *TIM2_PSC=0X00015999;
  *TIM2_ARR=0X00000999;
  *TIM2_CR1|=(1U<<0);//ENABLING TIMER
  *TIM2_DIER|=(1U<<0);//ENABLING UPDATE INTERRUPT SIMILAR TO UNMASKING

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void TIM2_IRQHandler(void){
	if(*TIM2_SR&0X01){//SIMILAR TO CHECKING PENDING STATUS OF INTERRUPT OCCURED
		*GPIOA_BSRR|=(1U<<5);
		for(uint32_t i=0;i<50000;i++);
		*GPIOA_BSRR|=(1U<<(16+5));
		*TIM2_SR&=~(1U<<0);//HAVE TO CLEAR PENDING STATUS
	}
}
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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
#ifdef USE_FULL_ASSERT
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
