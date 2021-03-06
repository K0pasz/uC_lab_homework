/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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
#include "gpio.h"

/* USER CODE BEGIN 0 */
#include "usart.h"
#include <stdio.h>
#include <string.h>
/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */
extern int digitindex0;
extern int digitindex1;
extern int digitindex2;
extern int digitindex3;

extern uint16_t steptimes [9999];
extern uint16_t steptimesindex;

extern uint16_t time;

extern char timestampstr[16];
extern char steps[32];
/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PAPin PA10 PA11 PA12 */
  GPIO_InitStruct.Pin = LD2_Pin|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PC8 PC9 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA8 PA9 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* USER CODE BEGIN 2 */
//Callback for the EXTI ITs
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	//If the sensor detects a step, it gives us an IT
	//Here we increase the step counter, save the timestamp and send it on USART to the PC
	if(GPIO_Pin == GPIO_PIN_9)
	{
		//increasing the step counter
		digitindex0++;

		//saving the timestamp
		steptimes[steptimesindex] = time;

		//putting the steps into a string
		//sprintf(steps, "%d ", steptimesindex+1);

		//sending it out on USART
		//HAL_UART_Transmit(&huart2, (uint8_t*) steps, strlen(steps), -1);

		//putting the time into a string
//		sprintf(timestampstr, "%d\n", time);


		//sending it out on USART
//		HAL_UART_Transmit(&huart2, (uint8_t*) timestampstr, strlen(timestampstr), -1);

		//of course, at last, we have to increment the index
		steptimesindex++;
	}

	//If we push the lower button we have to zero the steps and the timestamps
	if(GPIO_Pin == GPIO_PIN_7)
	{
		digitindex0 = 0;
		digitindex1 = 0;
		digitindex2 = 0;
		digitindex3 = 0;

		for(int i = 0; i<10000; i++)
		{
			steptimes[i] = 0;
		}

		//Optional:
		time = 0;
	}

	//If we push the upper button we send the timestamps out on USART
	if(GPIO_Pin == GPIO_PIN_6)
	{
		for(int i = 0; i<steptimesindex; i++)
		{
			//Putting the timestamps into a string and sending it out on UART

			sprintf(timestampstr, "%d\n", steptimes[i]);
			HAL_UART_Transmit(&huart2, (uint8_t*) timestampstr, strlen(timestampstr), -1);

		}
		//HAL_Delay(100); --> thanks to this, the Nucleo freezes...
		//DIY Delay:
		for(int k = 0; k < 2097151; k++)
		{

		}

	}
}
/* USER CODE END 2 */
