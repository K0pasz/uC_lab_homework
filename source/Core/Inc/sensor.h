#include "main.h"

extern SPI_HandleTypeDef hspi3;

extern uint8_t sensorINT1_CTRLAddress;
extern uint8_t sensorINT1_CTRLData;

extern uint8_t sensorTAPCFGAddress;
extern uint8_t sensorTAPCFGData;

extern uint8_t sensorCTRL10_CAddress;
extern uint8_t sensorCTRL10_CData;

extern uint8_t sensorCTRL1_XLAddress;
extern uint8_t sensorCTRL1_XLData;

extern uint8_t sensorSTEP_TIMESTAMP_LAddress;
extern uint8_t sensorSTEP_TIMESTAMP_HAddress;

extern uint8_t sensorSTEP_COUNT_DELTAAddress;
extern uint8_t sensorSTEP_COUNT_DELTAData;

extern uint8_t sensorINT2_CTRLAddress;
extern uint8_t sensorINT2_CTRLData;

void SensorInit();
