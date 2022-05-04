#include "sensor.h"

//Function to initialize the sensor
//We write the specific bits to the required registers
void SensorInit()
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi3, &sensorCTRL1_XLAddress, 1, -1);
	HAL_SPI_Transmit(&hspi3, &sensorCTRL1_XLData, 1, -1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi3, &sensorCTRL10_CAddress, 1, -1);
	HAL_SPI_Transmit(&hspi3, &sensorCTRL10_CData, 1, -1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi3, &sensorTAPCFGAddress, 1, -1);
	HAL_SPI_Transmit(&hspi3, &sensorTAPCFGData, 1, -1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi3, &sensorINT1_CTRLAddress, 1, -1);
	HAL_SPI_Transmit(&hspi3, &sensorINT1_CTRLData, 1, -1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi3, &sensorSTEP_COUNT_DELTAAddress, 1, -1);
	HAL_SPI_Transmit(&hspi3, &sensorSTEP_COUNT_DELTAData, 1, -1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi3, &sensorINT2_CTRLAddress, 1, -1);
	HAL_SPI_Transmit(&hspi3, &sensorINT2_CTRLData, 1, -1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);


}
