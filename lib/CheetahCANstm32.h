#ifndef CHEETAHCANSTM32_H
#define CHEETAHCANSTM32_H

#include "main.h"

void MX_CAN_Init(void);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *CanHandle);

CAN_HandleTypeDef hcan;
CAN_HandleTypeDef     CanHandle;
CAN_TxHeaderTypeDef   TxHeader;
CAN_RxHeaderTypeDef   RxHeader;
UART_HandleTypeDef huart1;
uint32_t              TxMailbox;
uint8_t               TxData[8];
uint8_t               RxData[8];



#endif
 
