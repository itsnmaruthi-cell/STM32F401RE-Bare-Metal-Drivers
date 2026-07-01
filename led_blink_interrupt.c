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
volatile uint8_t button_pressed=0;
//********using GPIOA/C ports
volatile uint32_t* RCC_AHB1ENR=(volatile uint32_t*)0X40023830;
volatile uint32_t* GPIOA_MODER=(volatile uint32_t*)0X40020000;
volatile uint32_t* GPIOA_BSRR=(volatile uint32_t*)0X40020018;
volatile uint32_t* GPIOC_MODER=(volatile uint32_t*)0X40020800;
//********INTERRUPS POINTERS********
//WE WILL GENERATE INTERRUPTS FROM PC13.SO INTERRUPTS ARE FROM LINE 13
//PC13 BEING A GPIO PIN,THE INTERRUPT TYPE IS EXTI
//BUT FIRST CLOCK ENABLE FOR SYSCFG IS NEEDED WHICH HAS EXTI CONFIGURATION
volatile uint32_t* RCC_APB2ENR=(volatile uint32_t*)0X40023844;
//LINE 13 EXTI IS IN EXTI13.EXTI 13 IS PART OF SYSCFG_EXTICR4
volatile uint32_t* SYSCFG_EXTICR4=(volatile uint32_t*)0X40013814;
//NVIC IS NECESSARY TO MANGING THIS INTERRUPT GENERATED
//EXTI LINE 13 INTERRUPT FALLS IN EXTI15_10_IRQHandler WITH IQR NO.40
//0-31 IS ISER0,32-63 IS ISER1.SO WE'LL USE ISER1
volatile uint32_t* NVIC_ISER1=(volatile uint32_t*)0XE000E104;
//WE NEED TO DO FURTHER CONFIGURATION EXTI SIMILAR TO MODER AND IDR/ODR USED FOR GPIO PINS
volatile uint32_t* EXTI_IMR=(volatile uint32_t*)0X40013C00;
volatile uint32_t* EXTI_FTSR=(volatile uint32_t*)0X40013C0C;
volatile uint32_t* EXTI_PR=(volatile uint32_t*)0X40013C14;







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
  *RCC_AHB1ENR|=((1U<<0)|(1U<<2));//PORTA AND C CLOCK ENABLE
  *RCC_APB2ENR|=(1U<<14);//SYSCFG CLOCK ENABLE
  *GPIOA_MODER&=~((3U<<10));//CLEARING BUTS
  *GPIOA_MODER|=(1U<<10);//PA5 OUTPUT
  *GPIOC_MODER&=~(3U<<26);//CLEARING AND INPUT PC13
  *SYSCFG_EXTICR4&=~(16U<<4);//CLEARING BITS
  *SYSCFG_EXTICR4|=(2U<<4);//TELLING LINE 13 IS FROM PC13
  *EXTI_FTSR|=(1U<<13);//ACTIVE LOW,SO SELECTING FALLING EDGE(1->0)TRIGGER
  *NVIC_ISER1|=(1U<<8);//40-32=8.THROUGH NVIC WE ARE TELLING CPU TO MANAGE THE INTERRUPS FROM LINE 13
  *EXTI_IMR|=(1U<<13);//AT LAST UNMASKING INTERRUPTS FROM LINE 13


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if(button_pressed){//POLLING IS USED(ALWAYS CHECKING button_pressed VARIABLE)
		  *GPIOA_BSRR|=(1U<<5);
		  for(uint32_t i=0;i<50000;i++);
		  button_pressed=0;
	  }
	  else{
		  *GPIOA_BSRR|=(1U<<(5+16));

	  }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void EXTI15_10_IRQHandler(void){
	if(((*EXTI_PR>>13)&0X01)){//ONCE BUTTON IS PRESSED PR IS 1
		button_pressed=1;//MAKING THIS VARIBLE 1
		*EXTI_PR|=(1U<<13);//NEED TO CLEAR PR BY WRITING 1,AS IT WRITE 1-ACTION TYPE REGISTER

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
