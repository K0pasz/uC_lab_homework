/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.c
  * @brief   This file provides code for the configuration
  *          of the TIM instances.
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
#include "tim.h"

/* USER CODE BEGIN 0 */
#include "spi.h"

extern int digitindex0;
extern int digitindex1;
extern int digitindex2;
extern int digitindex3;

extern uint8_t display0[11];
extern uint8_t display1[10];
extern uint8_t display2[10];
extern uint8_t display3[10];

extern GPIO_PinState is7segsel0;
extern GPIO_PinState is7segsel1;

extern uint16_t time;
/* USER CODE END 0 */

TIM_HandleTypeDef htim10;
TIM_HandleTypeDef htim11;

/* TIM10 init function */
void MX_TIM10_Init(void)
{

  /* USER CODE BEGIN TIM10_Init 0 */

  /* USER CODE END TIM10_Init 0 */

  /* USER CODE BEGIN TIM10_Init 1 */

  /* USER CODE END TIM10_Init 1 */
  htim10.Instance = TIM10;
  htim10.Init.Prescaler = 8000 - 1;
  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim10.Init.Period = 10 - 1;
  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM10_Init 2 */

  /* USER CODE END TIM10_Init 2 */

}
/* TIM11 init function */
void MX_TIM11_Init(void)
{

  /* USER CODE BEGIN TIM11_Init 0 */

  /* USER CODE END TIM11_Init 0 */

  /* USER CODE BEGIN TIM11_Init 1 */

  /* USER CODE END TIM11_Init 1 */
  htim11.Instance = TIM11;
  htim11.Init.Prescaler = 8000 - 1;
  htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim11.Init.Period = 10000 - 1;
  htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim11) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM11_Init 2 */

  /* USER CODE END TIM11_Init 2 */

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM10)
  {
  /* USER CODE BEGIN TIM10_MspInit 0 */

  /* USER CODE END TIM10_MspInit 0 */
    /* TIM10 clock enable */
    __HAL_RCC_TIM10_CLK_ENABLE();

    /* TIM10 interrupt Init */
    HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
  /* USER CODE BEGIN TIM10_MspInit 1 */

  /* USER CODE END TIM10_MspInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM11)
  {
  /* USER CODE BEGIN TIM11_MspInit 0 */

  /* USER CODE END TIM11_MspInit 0 */
    /* TIM11 clock enable */
    __HAL_RCC_TIM11_CLK_ENABLE();

    /* TIM11 interrupt Init */
    HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);
  /* USER CODE BEGIN TIM11_MspInit 1 */

  /* USER CODE END TIM11_MspInit 1 */
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM10)
  {
  /* USER CODE BEGIN TIM10_MspDeInit 0 */

  /* USER CODE END TIM10_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM10_CLK_DISABLE();

    /* TIM10 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM1_UP_TIM10_IRQn);
  /* USER CODE BEGIN TIM10_MspDeInit 1 */

  /* USER CODE END TIM10_MspDeInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM11)
  {
  /* USER CODE BEGIN TIM11_MspDeInit 0 */

  /* USER CODE END TIM11_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM11_CLK_DISABLE();

    /* TIM11 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM1_TRG_COM_TIM11_IRQn);
  /* USER CODE BEGIN TIM11_MspDeInit 1 */

  /* USER CODE END TIM11_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim10)
	{
		if(is7segsel1 == GPIO_PIN_RESET && is7segsel0 == GPIO_PIN_RESET)
			{
				Write7segsel(GPIO_PIN_RESET, GPIO_PIN_SET);

				HAL_SPI_Transmit(&hspi2, &display2[digitindex2], 1, 0);
			}

			if(is7segsel1 == GPIO_PIN_RESET && is7segsel0 == GPIO_PIN_SET)
			{
				Write7segsel(GPIO_PIN_SET, GPIO_PIN_RESET);

				HAL_SPI_Transmit(&hspi2, &display1[digitindex1], 1, 0);
			}

			if(is7segsel1 == GPIO_PIN_SET && is7segsel0 == GPIO_PIN_RESET)
			{
				Write7segsel(GPIO_PIN_SET, GPIO_PIN_SET);

				HAL_SPI_Transmit(&hspi2, &display0[digitindex0], 1, 0);
			}

			if(is7segsel1 == GPIO_PIN_SET && is7segsel0 == GPIO_PIN_SET)
			{
				Write7segsel(GPIO_PIN_RESET, GPIO_PIN_RESET);

				HAL_SPI_Transmit(&hspi2, &display3[digitindex3], 1, 0);
			}


			is7segsel0 = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11);
			is7segsel1 = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12);
		//LED villogtatás
		//HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		if(digitindex0 == 10)
		{
			digitindex0 = 0;

			if(digitindex1 != 9)
			{
				digitindex1++;
			}
			else
			{
				digitindex1 = 0;

				if(digitindex2 != 9)
				{
					digitindex2++;
				}
				else
				{
					digitindex2 = 0;

					if(digitindex3 != 9)
					{
						digitindex3++;
					}
					else
					{
						digitindex0 = 0;
						digitindex1 = 0;
						digitindex2 = 0;
						digitindex3 = 0;
					}
				}
			}
		}
	}

	if(htim == &htim11)
		{
			time++;
		}

}
/* USER CODE END 1 */
