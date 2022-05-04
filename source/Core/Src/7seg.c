#include "7seg.h"

void Write7segsel(GPIO_PinState sel1, GPIO_PinState sel0)
{
	if(sel0 == GPIO_PIN_SET)
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);

	if(sel1 == GPIO_PIN_SET)
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
}
