/*
 * telemetriarodas.h
 *
 *  Created on: Jul 6, 2021
 *      Author: caio
 */

#ifndef INC_TELEMETRIARODAS_H_
#define INC_TELEMETRIARODAS_H_

#include "stm32f1xx_hal.h"
#include "mlx90614.h"
#define DEBUG 1
#define IR_SENSOR_1 0x5C

void sendCANMessage(uint16_t id);
void writeRegister (uint8_t addr , uint8_t reg , uint16_t data);
uint16_t readRegister(uint8_t addr, uint8_t reg);
void setup();
void loop();

CAN_HandleTypeDef hcan;
extern I2C_HandleTypeDef hi2c2;
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;
CAN_TxHeaderTypeDef TxHeader;
uint32_t mailbox;
uint8_t mux;
uint8_t CAN_OUT[8];



#endif /* INC_TELEMETRIARODAS_H_ */
