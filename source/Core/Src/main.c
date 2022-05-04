/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "7seg.h"
#include "sensor.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

//Variables for the 7 segment numbers
const uint8_t digit0 = 0x3F;
const uint8_t digit1 = 0x06;
const uint8_t digit2 = 0x5B;
const uint8_t digit3 = 0x4F;
const uint8_t digit4 = 0x66;
const uint8_t digit5 = 0x6D;
const uint8_t digit6 = 0x7D;
const uint8_t digit7 = 0x07;
const uint8_t digit8 = 0x7F;
const uint8_t digit9 = 0x6F;
const uint8_t dummydigit = 0x00;

//Index variables for the 7 segment numbers
int digitindex0 = 0;
int digitindex1 = 0;
int digitindex2 = 0;
int digitindex3 = 0;

//Arrays for the 7 segment numbers
uint8_t display0[11] = {digit0, digit1, digit2, digit3, digit4, digit5, digit6, digit7, digit8, digit9, dummydigit};
uint8_t display1[10] = {digit0, digit1, digit2, digit3, digit4, digit5, digit6, digit7, digit8, digit9};
uint8_t display2[10] = {digit0, digit1, digit2, digit3, digit4, digit5, digit6, digit7, digit8, digit9};
uint8_t display3[10] = {digit0, digit1, digit2, digit3, digit4, digit5, digit6, digit7, digit8, digit9};

//We store the timestamps of the steps in steptimes ("only" 9999 becaues we only have 4 displays) and the index for it
uint16_t steptimes [9999];
uint16_t steptimesindex = 0;

//We save the timestamp into steptimes[steptimesindex] from this variable
//The tim11 timer increments this every 1 sec
uint16_t time = 0;

//String buffer for the USART communication
char timestampstr[16];

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

//Selection variables for the 4 displays
GPIO_PinState is7segsel0;
GPIO_PinState is7segsel1;

//Sensor register addresses and the required bits to write in them to work properly
//Pedometer step recognition interrupt enable on INT1 pad
uint8_t sensorINT1_CTRLAddress = 0x0D;
uint8_t sensorINT1_CTRLData = 0x80;

//Interrupts enable
uint8_t sensorTAPCFGAddress = 0x58;
uint8_t sensorTAPCFGData = 0x80;

//Timer enable, pedometer algorithm enable,tilt calculation enable and global enable bit for these functions
uint8_t sensorCTRL10_CAddress = 0x19;
uint8_t sensorCTRL10_CData = 0x3C;

//Performance configuration register
uint8_t sensorCTRL1_XLAddress = 0x10;
uint8_t sensorCTRL1_XLData = 0x20;

//Step timestamp lower and higher byte addresses
uint8_t sensorSTEP_TIMESTAMP_LAddress = 0xC9;
uint8_t sensorSTEP_TIMESTAMP_HAddress = 0xCA;

//Time period register for step detection on delta time
uint8_t sensorSTEP_COUNT_DELTAAddress = 0x15;
uint8_t sensorSTEP_COUNT_DELTAData = 0xFF;

//Step recognition interrupt on delta time on INT2 pad
uint8_t sensorINT2_CTRLAddress = 0x0E;
uint8_t sensorINT2_CTRLData = 0x80;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
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
  MX_SPI2_Init();
  MX_USART2_UART_Init();
  MX_SPI3_Init();
  MX_TIM10_Init();
  MX_TIM11_Init();
  /* USER CODE BEGIN 2 */

  //Starting the timers with interrupts
  HAL_TIM_Base_Start_IT(&htim10);
  HAL_TIM_Base_Start_IT(&htim11);

  //Disabling the CS for the sensor
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);

  //Initializing the sensor
  SensorInit();

  //Enable the 7seg displays (we always want to see the steps on it)
  //Shift register LE
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);

  //Shift register OE
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  //We implemented everything in interrupts
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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
