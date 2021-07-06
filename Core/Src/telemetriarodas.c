/*
 * telemetriarodas.c
 *
 *  Created on: Jul 6, 2021
 *      Author: caio
 */

#include "telemetriarodas.h"
#include "stdio.h"

uint32_t               uwIC2Value1 = 0;
uint32_t               uwIC2Value2 = 0;
uint32_t               uwDiffCapture = 0;
uint16_t               uhCaptureIndex = 0;
uint16_t               uwFrequency = 0;

void setup()
{
	mux = 1;
	TxHeader.StdId = 0x0;
	TxHeader.ExtId = 0x0;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.RTR = CAN_RTR_DATA ;
	TxHeader.DLC = 8;
	TxHeader.TransmitGlobalTime = DISABLE;

	CAN_FilterTypeDef sFilterConfig;
	sFilterConfig.FilterIdHigh = 0x0;
	sFilterConfig.FilterIdLow = 0x0;
	sFilterConfig.FilterMaskIdHigh = 0x0;
	sFilterConfig.FilterMaskIdLow = 0x0;
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	sFilterConfig.FilterBank = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterActivation = ENABLE;
	sFilterConfig.SlaveStartFilterBank = 14;
	HAL_CAN_ConfigFilter(&hcan , &sFilterConfig);
	HAL_CAN_Start(&hcan);
	HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
	if(DEBUG)
	{
		HAL_StatusTypeDef result;
		char temp_buff[128] = {};

		for (int i = 0; i<128; i++)
		{
			result = HAL_I2C_IsDeviceReady(&hi2c2, (uint16_t) (i<<1), 2, 2);
			if (result != HAL_OK)
			{
			  sprintf(temp_buff, ".");
			  HAL_UART_Transmit(&huart1 , temp_buff ,  strlen((const char *)temp_buff) , 100);
			}
			if (result == HAL_OK)
			{
			  sprintf(temp_buff, "0x%X", i);
			  HAL_UART_Transmit(&huart1 , temp_buff ,  strlen((const char *)temp_buff) , 100);
			}
		}
		  if(HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_3) != HAL_OK)
		  {
		    /* Starting Error */
		    Error_Handler();
		  }
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

}



void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
  {
    if(uhCaptureIndex == 0)
    {
      /* Get the 1st Input Capture value */
      uwIC2Value1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);
      uhCaptureIndex = 1;
    }
    else if(uhCaptureIndex == 1)
    {
      /* Get the 2nd Input Capture value */
      uwIC2Value2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);

      /* Capture computation */
      if (uwIC2Value2 > uwIC2Value1)
      {
        uwDiffCapture = (uwIC2Value2 - uwIC2Value1);
      }
      else if (uwIC2Value2 < uwIC2Value1)
      {
        /* 0xFFFF is max TIM2_CCRx value */
        uwDiffCapture = ((0xFFFF - uwIC2Value1) + uwIC2Value2) + 1;
      }
      else
      {
        /* If capture values are equal, we have reached the limit of frequency
           measures */
        Error_Handler();
      }
      /* Frequency computation: for this example TIMx (TIM2) is clocked by
         APB1Clk */
//      uwFrequency = HAL_RCC_GetPCLK1Freq() / uwDiffCapture;
      uwFrequency = uwDiffCapture;
      uhCaptureIndex = 0;
    }
  }
}

void sendCANMessage(uint16_t id)
{
	TxHeader.StdId = id;
	TxHeader.ExtId = 0x0;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.RTR = CAN_RTR_DATA ;
	TxHeader.DLC = 8;
	TxHeader.TransmitGlobalTime = DISABLE;

	HAL_CAN_AddTxMessage(&hcan , &TxHeader , CAN_OUT , &mailbox);
}
uint16_t cont = 0;

void loop()
{
	unsigned char saida[400] = "";

	uint16_t temp_obj1 = MLX90614_ReadReg(IR_SENSOR_1, MLX90614_TOBJ1, MLX90614_DBG_ON);
	CAN_OUT[0] = temp_obj1 & 0xFF;
	CAN_OUT[1] = temp_obj1 >> 8;
    CAN_OUT[2] = uwFrequency & 0xFF;
	CAN_OUT[3] = uwFrequency >> 8;
	uint16_t contatual = __HAL_TIM_GET_COUNTER(&htim1);
	CAN_OUT[4] = contatual & 0xFF;
	CAN_OUT[5] = contatual >> 8;
	sendCANMessage(0x1);
	uint16_t temp = temp_obj1*2 - 27315;
	sprintf(saida , "Temperatura = 0x%d \n \r" , temp);
	if(DEBUG)
			HAL_UART_Transmit(&huart1, saida ,sizeof(saida), 100);
	HAL_Delay(500);
}
