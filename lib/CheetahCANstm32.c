 #include "CheetahCANstm32.h"

void MX_CAN_Init(void)
{
  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 4;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_15TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = DISABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = DISABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
  CAN_FilterTypeDef  sFilterConfig;
   sFilterConfig.FilterBank = 0;
   sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
   sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
   sFilterConfig.FilterIdHigh = 0x0000;
   sFilterConfig.FilterIdLow = 0x0000;
   sFilterConfig.FilterMaskIdHigh = 0x0000;
   sFilterConfig.FilterMaskIdLow = 0x0000;
   sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
   sFilterConfig.FilterActivation = ENABLE;
   sFilterConfig.SlaveStartFilterBank = 14;

   if (HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)
   {
     Error_Handler();
   }

   if (HAL_CAN_Start(&hcan) != HAL_OK)
   {
     Error_Handler();
   }
   if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
   {
     Error_Handler();
   }
   TxHeader.StdId = 0x321;
   TxHeader.ExtId = 0x01;
   TxHeader.RTR = CAN_RTR_DATA;
   TxHeader.IDE = CAN_ID_STD;
   TxHeader.DLC = 8;
   TxHeader.TransmitGlobalTime = DISABLE;
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *CanHandle)
{
  if (HAL_CAN_GetRxMessage(CanHandle, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
  {
    Error_Handler();
  }
  uint8_t saida[10];
  for(int i = 1 ; i < 9 ; i++)
  {
	  saida[i] = RxData[i-1];
  }
  saida[0] = 0xFF;
  saida[9] = 0xFE;
  HAL_UART_Transmit(&huart1, saida ,sizeof(saida), 100);
}
