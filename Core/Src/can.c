/**
  ******************************************************************************
  * File Name          : CAN.c
  * Description        : This file provides code for the configuration
  *                      of the CAN instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "can.h"

/* USER CODE BEGIN 0 */
#include "usbd_cdc_if.h"
#include "canopen_object_dict.h"

/************************************************************************************************
 GLOBAL VARIABLES
 ************************************************************************************************/
CanDataFrameInit can_frame_template;
CAN_FilterTypeDef can_filter_template;
CAN_RxHeaderTypeDef can_rx_header;
CAN_TxHeaderTypeDef can_tx_header;
uint8_t can_rx_data[8];
uint8_t TxData[8];
uint8_t count = 0;
uint32_t TxMailbox;
CanDataFrameInit can_rx_frame_template;
bool error;
bool charging;
bool driving;
uint32_t can_tx_mailbox;

/* USER CODE END 0 */

CAN_HandleTypeDef hcan1;
CAN_HandleTypeDef hcan2;

/* CAN1 init function */
void MX_CAN1_Init(void)
{

  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 10;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_13TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }

}
/* CAN2 init function */
void MX_CAN2_Init(void)
{

  hcan2.Instance = CAN2;
  hcan2.Init.Prescaler = 50;
  hcan2.Init.Mode = CAN_MODE_NORMAL;
  hcan2.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan2.Init.TimeSeg1 = CAN_BS1_13TQ;
  hcan2.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan2.Init.TimeTriggeredMode = DISABLE;
  hcan2.Init.AutoBusOff = DISABLE;
  hcan2.Init.AutoWakeUp = DISABLE;
  hcan2.Init.AutoRetransmission = DISABLE;
  hcan2.Init.ReceiveFifoLocked = DISABLE;
  hcan2.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan2) != HAL_OK)
  {
    Error_Handler();
  }

}

static uint32_t HAL_RCC_CAN1_CLK_ENABLED=0;

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    HAL_RCC_CAN1_CLK_ENABLED++;
    if(HAL_RCC_CAN1_CLK_ENABLED==1){
      __HAL_RCC_CAN1_CLK_ENABLE();
    }

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**CAN1 GPIO Configuration
    PA11     ------> CAN1_RX
    PA12     ------> CAN1_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(CAN1_TX_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
    HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
  else if(canHandle->Instance==CAN2)
  {
  /* USER CODE BEGIN CAN2_MspInit 0 */

  /* USER CODE END CAN2_MspInit 0 */
    /* CAN2 clock enable */
    __HAL_RCC_CAN2_CLK_ENABLE();
    HAL_RCC_CAN1_CLK_ENABLED++;
    if(HAL_RCC_CAN1_CLK_ENABLED==1){
      __HAL_RCC_CAN1_CLK_ENABLE();
    }

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**CAN2 GPIO Configuration
    PB12     ------> CAN2_RX
    PB13     ------> CAN2_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* CAN2 interrupt Init */
    HAL_NVIC_SetPriority(CAN2_TX_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(CAN2_TX_IRQn);
    HAL_NVIC_SetPriority(CAN2_RX0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);
    HAL_NVIC_SetPriority(CAN2_RX1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN2_RX1_IRQn);
  /* USER CODE BEGIN CAN2_MspInit 1 */

  /* USER CODE END CAN2_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    HAL_RCC_CAN1_CLK_ENABLED--;
    if(HAL_RCC_CAN1_CLK_ENABLED==0){
      __HAL_RCC_CAN1_CLK_DISABLE();
    }

    /**CAN1 GPIO Configuration
    PA11     ------> CAN1_RX
    PA12     ------> CAN1_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

    /* CAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(CAN1_TX_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_RX1_IRQn);
  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
  else if(canHandle->Instance==CAN2)
  {
  /* USER CODE BEGIN CAN2_MspDeInit 0 */

  /* USER CODE END CAN2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN2_CLK_DISABLE();
    HAL_RCC_CAN1_CLK_ENABLED--;
    if(HAL_RCC_CAN1_CLK_ENABLED==0){
      __HAL_RCC_CAN1_CLK_DISABLE();
    }

    /**CAN2 GPIO Configuration
    PB12     ------> CAN2_RX
    PB13     ------> CAN2_TX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_12|GPIO_PIN_13);

    /* CAN2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(CAN2_TX_IRQn);
    HAL_NVIC_DisableIRQ(CAN2_RX0_IRQn);
    HAL_NVIC_DisableIRQ(CAN2_RX1_IRQn);
  /* USER CODE BEGIN CAN2_MspDeInit 1 */

  /* USER CODE END CAN2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/************************************************************************************************
 FUNCTIONS CREATED OUTSIDE CUBEMX
 ************************************************************************************************/

/**
 * @brief: Initialize CAN network
 * @param chosen_network
 *
 **/
void CanInit(CAN_HandleTypeDef chosen_network) {
	if (HAL_CAN_Start(&chosen_network) != HAL_OK) {
		Error_Handler();
	}

	if (HAL_CAN_ActivateNotification(&chosen_network,
	CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief: store received data from chosen network
 * @param chosen_network
 *
 **/
void CanReceivedData(CAN_HandleTypeDef chosen_network, CanDataFrameInit *ptr_can_rx_frame_template) {
	if (HAL_CAN_GetRxMessage(&chosen_network, CAN_RX_FIFO0, &ptr_can_rx_frame_template->rx_header,
			ptr_can_rx_frame_template->rx_data) != HAL_OK) {
		/* Reception Error */
		Error_Handler();
	}
	CanClearRxDataFrame(ptr_can_rx_frame_template);
}

CanDataFrameInit CanSaveReceivedData(CAN_HandleTypeDef chosen_network, CanDataFrameInit *ptr_can_rx_frame_template) {
	if (HAL_CAN_GetRxMessage(&chosen_network, CAN_RX_FIFO0, &ptr_can_rx_frame_template->rx_header,
			ptr_can_rx_frame_template->rx_data) != HAL_OK) {
		/* Reception Error */
		Error_Handler();
	}
	return *ptr_can_rx_frame_template;
}

/**
 * @brief: configure can data frames hardware filter
 * @param chosen_network
 * @param can_filter_bank: Specific filter bank from 0-14
 * @param can_filter_id_high: High byte of CAN ID to be received
 * @param can_filter_id_low: Low byte of CAN ID to be received
 * @param can_filter_mask_id_high: High byte of CAN ID mask - IDs to be received
 * @param can_filter_mask_id_low: Low byte of CAN ID mask - IDs to be received
 *
 **/
void CanConfigFilter(CAN_HandleTypeDef chosen_network, uint8_t can_filter_bank,
		uint32_t can_filter_id_high, uint32_t can_filter_id_low,
		uint32_t can_filter_mask_id_high, uint32_t can_filter_mask_id_low) {
	can_filter_template.FilterBank = can_filter_bank;
	can_filter_template.FilterMode = CAN_FILTERMODE_IDMASK;
	can_filter_template.FilterScale = CAN_FILTERSCALE_32BIT;
//	can_filter_template.FilterIdHigh = 0x290 << 5; //can_filter_id_high; //18FF;			//0x321 << 5;
//	can_filter_template.FilterIdLow = 0x00000000; //can_filter_id_low; //50E5;				//0x00000000;
	can_filter_template.FilterIdHigh = 0x0000;
	can_filter_template.FilterIdLow = 0x0000;
	can_filter_template.FilterMaskIdHigh = 0x0000;
	can_filter_template.FilterMaskIdLow = 0x0000;
//	can_filter_template.FilterMaskIdHigh = 0x290 << 5;	//0x111 << 5;
//	can_filter_template.FilterMaskIdLow = 0x00000000;
	can_filter_template.FilterFIFOAssignment = CAN_RX_FIFO0;
	can_filter_template.FilterActivation = ENABLE;
	can_filter_template.SlaveStartFilterBank = 14;

	if (HAL_CAN_ConfigFilter(&chosen_network, &can_filter_template) != HAL_OK) {
		Error_Handler();
	}

}

/**
 * @brief: synchronize data from nodes with canopen sync frame
 * @param chosen_network
 * @param ptr_can_frame_template: pointer to a structure with basic can frame parameteres
 *
 */
void CanSendSync(CAN_HandleTypeDef chosen_network,
		CanDataFrameInit *ptr_can_frame_template) {
	ptr_can_frame_template->tx_header.StdId = 0x080;
	ptr_can_frame_template->tx_header.RTR = CAN_RTR_DATA;
	ptr_can_frame_template->tx_header.IDE = CAN_ID_STD;
	ptr_can_frame_template->tx_header.DLC = 0;
	ptr_can_frame_template->tx_header.TransmitGlobalTime = DISABLE;

	if (HAL_CAN_AddTxMessage(&chosen_network,
			&ptr_can_frame_template->tx_header, ptr_can_frame_template->tx_data,
			&can_tx_mailbox) != HAL_OK) {
		Error_Handler();
	}
	while (HAL_CAN_GetTxMailboxesFreeLevel(&chosen_network) != 3) {
	}
	CanClearTxDataFrame(ptr_can_frame_template);
}

/**
 * @brief: perform network management with canopen nmt frame
 * @param chosen_network
 * @param state: one from global defined states available in ./Core/Src/can.h
 * @param node_id: chosen node to receive the order
 * @param ptr_can_frame_template: pointer to a structure with basic can frame parameteres
 *
 */
void CanSendNmt(CAN_HandleTypeDef chosen_network, uint8_t state,
		uint8_t node_id, CanDataFrameInit *ptr_can_frame_template) {
	ptr_can_frame_template->tx_header.StdId = 0x000;
	ptr_can_frame_template->tx_header.RTR = CAN_RTR_DATA;
	ptr_can_frame_template->tx_header.IDE = CAN_ID_STD;
	ptr_can_frame_template->tx_header.DLC = 2;
	ptr_can_frame_template->tx_header.TransmitGlobalTime = DISABLE;

	ptr_can_frame_template->tx_data[0] = state;
	ptr_can_frame_template->tx_data[1] = node_id;

	if (HAL_CAN_AddTxMessage(&chosen_network,
			&ptr_can_frame_template->tx_header, ptr_can_frame_template->tx_data,
			&can_tx_mailbox) != HAL_OK) {
		Error_Handler();
	}
	while (HAL_CAN_GetTxMailboxesFreeLevel(&chosen_network) != 3) {
	}
	CanClearTxDataFrame(ptr_can_frame_template);
}

/**
 * @brief Transfer CAN PDO frame to chosen network
 * @param chosen_network: CAN_HIGH_SPEED or CAN_LOW_SPEED
 * @param frame_pdo_id: choose node's Transmit PDO or Receive PDO ID
 * @param number_of_bytes: between 0-8
 * @param data_to_be_sent: array storing the sent data
 * @param *can_frame_template: pointer to a structure containing basic frame parameters
 *
 **/
void CanTransferFrame(CAN_HandleTypeDef chosen_network, CanDataFrameInit *ptr_can_frame_template)
{
	ptr_can_frame_template->tx_header.StdId = ptr_can_frame_template->rx_header.StdId;
	ptr_can_frame_template->tx_header.DLC = ptr_can_frame_template->rx_header.DLC;
	ptr_can_frame_template->tx_data[0] = ptr_can_frame_template->rx_data[0];
	ptr_can_frame_template->tx_data[1] = ptr_can_frame_template->rx_data[1];
	ptr_can_frame_template->tx_data[2] = ptr_can_frame_template->rx_data[2];
	ptr_can_frame_template->tx_data[3] = ptr_can_frame_template->rx_data[3];
	ptr_can_frame_template->tx_data[4] = ptr_can_frame_template->rx_data[4];
	ptr_can_frame_template->tx_data[5] = ptr_can_frame_template->rx_data[5];
	ptr_can_frame_template->tx_data[6] = ptr_can_frame_template->rx_data[6];
	ptr_can_frame_template->tx_data[7] = ptr_can_frame_template->rx_data[7];

	if (HAL_CAN_AddTxMessage(&chosen_network,
			&ptr_can_frame_template->tx_header, ptr_can_frame_template->tx_data,
			&can_tx_mailbox) != HAL_OK) {
		Error_Handler();
	}

	while (HAL_CAN_GetTxMailboxesFreeLevel(&chosen_network) != 3) {
	}
	CanClearTxDataFrame(ptr_can_frame_template);
}

void CanSendPdo(CAN_HandleTypeDef chosen_network, uint32_t frame_pdo_id,
		uint8_t number_of_bytes, CanDataFrameInit *ptr_can_frame_template,
		uint8_t byte0, uint8_t byte1, uint8_t byte2, uint8_t byte3,
		uint8_t byte4, uint8_t byte5, uint8_t byte6, uint8_t byte7) {
	ptr_can_frame_template->tx_header.StdId = frame_pdo_id;
	ptr_can_frame_template->tx_header.RTR = CAN_RTR_DATA;
	ptr_can_frame_template->tx_header.IDE = CAN_ID_STD;
	ptr_can_frame_template->tx_header.DLC = number_of_bytes;
	ptr_can_frame_template->tx_header.TransmitGlobalTime = DISABLE;
	ptr_can_frame_template->tx_data[0] = byte0;
	ptr_can_frame_template->tx_data[1] = byte1;
	ptr_can_frame_template->tx_data[2] = byte2;
	ptr_can_frame_template->tx_data[3] = byte3;
	ptr_can_frame_template->tx_data[4] = byte4;
	ptr_can_frame_template->tx_data[5] = byte5;
	ptr_can_frame_template->tx_data[6] = byte6;
	ptr_can_frame_template->tx_data[7] = byte7;

	if (HAL_CAN_AddTxMessage(&chosen_network,
			&ptr_can_frame_template->tx_header, ptr_can_frame_template->tx_data,
			&can_tx_mailbox) != HAL_OK) {
		Error_Handler();
	}

	while (HAL_CAN_GetTxMailboxesFreeLevel(&chosen_network) != 3) {
	}

	CanClearTxDataFrame(ptr_can_frame_template);

}

CanDataFrameInit CanMakeFrameWithValue(CanDataFrameInit *CanFrame, uint8_t frameId, uint8_t byte0, uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, uint8_t byte5, uint8_t byte6, uint8_t byte7)
{
	CanFrame->rx_header.StdId = frameId;
	CanFrame->rx_header.RTR = CAN_RTR_DATA;
	CanFrame->rx_header.IDE = CAN_ID_STD;
	CanFrame->rx_header.DLC = 8;
	CanFrame->rx_data[0] = byte0;
	CanFrame->rx_data[1] = byte1;
	CanFrame->rx_data[2] = byte2;
	CanFrame->rx_data[3] = byte3;
	CanFrame->rx_data[4] = byte4;
	CanFrame->rx_data[5] = byte5;
	CanFrame->rx_data[6] = byte6;
	CanFrame->rx_data[7] = byte7;
	return *CanFrame;
}

/**
 * @brief Transfer CAN SDO frame to chosen network
 * @param chosen_network: CAN_HIGH_SPEED or CAN_LOW_SPEED
 * @param frame_sdo_id: choose node's Transmit SDO or Receive SDO ID
 * @param number_of_bytes: between 0-7
 * @param bytes0-7: data
 * @param *ptr_can_frame_template: pointer to a structure containing basic frame parameters
 *
 **/
void CanSendSdo(CAN_HandleTypeDef chosen_network, uint32_t frame_sdo_id,
		CanDataFrameInit *ptr_can_frame_template, uint8_t number_of_bytes,
		uint8_t command_byte, uint8_t byte0, uint8_t byte1, uint8_t byte2,
		uint8_t byte3, uint8_t byte4, uint8_t byte5, uint8_t byte6) {
	ptr_can_frame_template->tx_header.StdId = frame_sdo_id;
	ptr_can_frame_template->tx_header.RTR = CAN_RTR_DATA;
	ptr_can_frame_template->tx_header.IDE = CAN_ID_STD;
	ptr_can_frame_template->tx_header.DLC = number_of_bytes;
	ptr_can_frame_template->tx_header.TransmitGlobalTime = DISABLE;
	ptr_can_frame_template->tx_data[0] = command_byte;
	ptr_can_frame_template->tx_data[1] = byte0;
	ptr_can_frame_template->tx_data[2] = byte1;
	ptr_can_frame_template->tx_data[3] = byte2;
	ptr_can_frame_template->tx_data[4] = byte3;
	ptr_can_frame_template->tx_data[5] = byte4;
	ptr_can_frame_template->tx_data[6] = byte5;
	ptr_can_frame_template->tx_data[7] = byte6;

	if (HAL_CAN_AddTxMessage(&chosen_network,
			&ptr_can_frame_template->tx_header, ptr_can_frame_template->tx_data,
			&can_tx_mailbox) != HAL_OK) {
		Error_Handler();
	}

	while (HAL_CAN_GetTxMailboxesFreeLevel(&chosen_network) != 3) {
	}

	CanClearTxDataFrame(ptr_can_frame_template);

}

void StartCanCommunication()
{
	CanSendNmt(CAN_HIGH_SPEED, OPERATIONAL_STATE, bms.node_id,
		&can_frame_template);
	HAL_Delay(1);
	CanSendNmt(CAN_HIGH_SPEED, OPERATIONAL_STATE, inverter_1.node_id,
			&can_frame_template);
	HAL_Delay(1);
	CanSendNmt(CAN_HIGH_SPEED, OPERATIONAL_STATE, inverter_2.node_id,
			&can_frame_template);
	HAL_Delay(1);
	CanSendNmt(CAN_HIGH_SPEED, OPERATIONAL_STATE, mppt_1.node_id,
			&can_frame_template);
	HAL_Delay(1);
	CanSendNmt(CAN_HIGH_SPEED, OPERATIONAL_STATE, mppt_2.node_id,
			&can_frame_template);
	HAL_Delay(1);
	CanSendNmt(CAN_HIGH_SPEED, OPERATIONAL_STATE, mppt_3.node_id,
			&can_frame_template);
	HAL_Delay(1);
	CanSendNmt(CAN_HIGH_SPEED, OPERATIONAL_STATE, lights_controller.node_id,
			&can_frame_template);
	HAL_Delay(1);
	CanSendNmt(CAN_HIGH_SPEED, OPERATIONAL_STATE, dashboard.node_id,
			&can_frame_template);
	HAL_Delay(1);
}

void StopCanCommunication()
{
	CanSendNmt(CAN_HIGH_SPEED, STOPPED_STATE, bms.node_id,
			&can_frame_template);
	HAL_Delay(1);
	CanSendNmt(CAN_HIGH_SPEED, STOPPED_STATE, inverter_1.node_id,
			&can_frame_template);
	HAL_Delay(1);
	CanSendNmt(CAN_HIGH_SPEED, STOPPED_STATE, inverter_2.node_id,
			&can_frame_template);
	HAL_Delay(1);
	CanSendNmt(CAN_HIGH_SPEED, STOPPED_STATE, mppt_1.node_id,
			&can_frame_template);
	HAL_Delay(1);
	CanSendNmt(CAN_HIGH_SPEED, STOPPED_STATE, mppt_2.node_id,
			&can_frame_template);
	HAL_Delay(1);
	CanSendNmt(CAN_HIGH_SPEED, STOPPED_STATE, mppt_3.node_id,
			&can_frame_template);
	HAL_Delay(1);
	CanSendNmt(CAN_HIGH_SPEED, STOPPED_STATE, lights_controller.node_id,
			&can_frame_template);
	HAL_Delay(1);
	CanSendNmt(CAN_HIGH_SPEED, STOPPED_STATE, dashboard.node_id,
			&can_frame_template);
	HAL_Delay(1);
}

/**
 * @brief Transfer specific CAN data frame to selected low or high speed network
 * @param hcanx: chosen network
 * @param sender_id: received ID
 * @param receiver_id: node chosen to receive
 *
 * TODO(szymon): adaptacja do wersji programu ze wskaznikiem na strukture ramki
 **/
/*
void CanTransfer(CAN_HandleTypeDef hcanx, CanDataFrameInit *canFrame, uint32_t sender_id) {
	if ((can_rx_header.StdId == sender_id)
			&& (can_rx_header.RTR == CAN_RTR_DATA)
			&& (can_rx_header.IDE == CAN_ID_STD)) {
		for (int i = 0; i <= can_rx_header.DLC - 1; i++) {
			can_rx_frame_template.rx_data[i] = can_rx_data[i];
		}
	} else {
		HAL_GPIO_TogglePin(LED_D6_GPIO_Port, LED_D6_Pin);
	}
	canFrame->tx_header.DLC = canFrame->rx_header.DLC;

	if (HAL_CAN_AddTxMessage(&hcanx, canFrame->tx_header,
			canFrame->rx_data, &can_tx_mailbox) != HAL_OK) {
		Error_Handler();
	}
}
*/
/**
 * @brief: data sent over usb is not correctly shown when structure is not cleared
 *         after every message sent. Assigning zeros has no influence on the network
 * @param ptr_can_frame_template: chosen structure which helds all the data
 *
 **/
void CanClearTxDataFrame(CanDataFrameInit *ptr_can_frame_template) {
	ptr_can_frame_template->tx_header.StdId = 0x00;
	ptr_can_frame_template->tx_header.RTR = CAN_RTR_DATA;
	ptr_can_frame_template->tx_header.IDE = CAN_ID_STD;
	ptr_can_frame_template->tx_header.DLC = 0;
	ptr_can_frame_template->tx_header.TransmitGlobalTime = DISABLE;

	ptr_can_frame_template->tx_data[0] = 0x0;
	ptr_can_frame_template->tx_data[1] = 0x0;
	ptr_can_frame_template->tx_data[2] = 0x0;
	ptr_can_frame_template->tx_data[3] = 0x0;
	ptr_can_frame_template->tx_data[4] = 0x0;
	ptr_can_frame_template->tx_data[5] = 0x0;
	ptr_can_frame_template->tx_data[6] = 0x0;
	ptr_can_frame_template->tx_data[7] = 0x0;
}


void CanClearRxDataFrame(CanDataFrameInit *ptr_can_frame_template) {
	ptr_can_frame_template->rx_header.StdId = 0x00;
	ptr_can_frame_template->rx_header.ExtId = 0x00;
	ptr_can_frame_template->rx_header.RTR = CAN_RTR_DATA;
	ptr_can_frame_template->rx_header.IDE = CAN_ID_STD;
	ptr_can_frame_template->rx_header.DLC = 0;

	ptr_can_frame_template->rx_data[0] = 0x0;
	ptr_can_frame_template->rx_data[1] = 0x0;
	ptr_can_frame_template->rx_data[2] = 0x0;
	ptr_can_frame_template->rx_data[3] = 0x0;
	ptr_can_frame_template->rx_data[4] = 0x0;
	ptr_can_frame_template->rx_data[5] = 0x0;
	ptr_can_frame_template->rx_data[6] = 0x0;
	ptr_can_frame_template->rx_data[7] = 0x0;
}

void CanSendExtendedIdMessage(CAN_HandleTypeDef chosen_network,
		CanDataFrameInit *ptr_can_frame_template, uint32_t FrameId, uint8_t DLC,
		uint8_t byte0, uint8_t byte1, uint8_t byte2, uint8_t byte3,
		uint8_t byte4, uint8_t byte5, uint8_t byte6, uint8_t byte7)
{
	ptr_can_frame_template->tx_header.ExtId = FrameId;
	ptr_can_frame_template->tx_header.RTR = CAN_RTR_DATA;
	ptr_can_frame_template->tx_header.IDE = CAN_ID_EXT;
	ptr_can_frame_template->tx_header.DLC = DLC;
	ptr_can_frame_template->tx_header.TransmitGlobalTime = DISABLE;
	ptr_can_frame_template->tx_data[0] = byte0;
	ptr_can_frame_template->tx_data[1] = byte1;
	ptr_can_frame_template->tx_data[2] = byte2;
	ptr_can_frame_template->tx_data[3] = byte3;
	ptr_can_frame_template->tx_data[4] = byte4;
	ptr_can_frame_template->tx_data[5] = byte5;
	ptr_can_frame_template->tx_data[6] = byte6;
	ptr_can_frame_template->tx_data[7] = byte7;

	if (HAL_CAN_AddTxMessage(&chosen_network,
		&ptr_can_frame_template->tx_header, ptr_can_frame_template->tx_data,
		&can_tx_mailbox) != HAL_OK) {
	Error_Handler();
	}

	while (HAL_CAN_GetTxMailboxesFreeLevel(&chosen_network) != 3) {
	}

	CanClearTxDataFrame(ptr_can_frame_template);
}

/*My math mini library BEGIN*/
void testMath(int to_test)
{
	if(to_test == 3000){
		CanSendPdo(hcan1, 0x69, 2, &can_frame_template, 6, 9, 0, 0, 0, 0, 0, 0);
	}
}

int unParse2Bytes(uint8_t lowerByte, uint8_t higherByte)
{
	int cas = ((int)lowerByte + (int)higherByte * byteMaxValue);
	return cas;
}

GluedBytes Parse2Bytes(int toParse)
{
	GluedBytes parsed16;
	parsed16.lowerByte = toParse % byteMaxValue;
	parsed16.higherByte = (toParse / byteMaxValue) % byteMaxValue;
	return parsed16;
}

GluedBytes MakeGluedBytes(uint8_t lowerByte, uint8_t higherByte)
{
	GluedBytes bytes;
	bytes.lowerByte = lowerByte;
	bytes.higherByte = higherByte;
	return bytes;
}
/*My math mini library END*/

/*CAN COMMUNICATION SECTION BEGIN*/
/*CAR STATES MODULES*/
//void StartCharging(CanDataFrameInit *canFrame)
//{
//	if (charging == false)
//	{
//		if (canFrame->rx_header.ExtId == 0x18FF50E5)
//		{
//
//		}
//	}
//		charging = canFrame->rx_header.ExtId == 0x18FF50E5;
//}

void ChargingStateModule()
{
	if (charging == true && error == false)
	{
		CanSendExtendedIdMessage(hcan1, &can_frame_template, 0x1806E5F4, 8,
				0x03, 0xE8, 0, 10, 0, 0, 0, 0);

		CanClearRxDataFrame(&can_rx_frame_template);
		HAL_Delay(1000);
		if (error == true)
		{
			StopCanCommunication();
		}
	}
}

void DrivingStateModule()
{
	StopCanCommunication();
	HAL_Delay(2);
	StartCanCommunication();
	while(driving && !error)
	{
		HandleHighSpeed();

		HandleLowSpeed();
	}
	if(error)
	{
		StopCanCommunication();
	}

}

void HandleHighSpeed()
{
	CanSaveReceivedData(CAN_HIGH_SPEED, &can_rx_frame_template);
	UsbTransfer(&can_rx_frame_template);
	CatchErrorOccuring(&can_rx_frame_template);
	CanClearRxDataFrame(&can_frame_template);
}

void HandleLowSpeed()
{
	CanSaveReceivedData(CAN_LOW_SPEED, &can_rx_frame_template);
	UsbTransfer(&can_rx_frame_template);
	CatchErrorOccuring(&can_rx_frame_template);
	CanClearRxDataFrame(&can_rx_frame_template);
}

/*Only optional to talk out with people*/
void ParkingStateModule()
{

}

/*CHARGING ACTIONS BEGIN*/
void ActUponCurrentAndVoltage(CanDataFrameInit *canFrame, int maxVoltage, int maxCurrent)
{
	int voltage = unParse2Bytes(canFrame->rx_data[1], canFrame->rx_data[0]);
	int current = unParse2Bytes(canFrame->rx_data[3], canFrame->rx_data[2]);
	testMath(voltage);
	error = ( (voltage>maxVoltage) || (current>maxCurrent) );
}

void CatchChargingErrorOccuring(CanDataFrameInit *canFrame)
{
	error = ( canFrame->rx_data[4] != 0 );
}
/*CHARGING ACTIONS END*/

/*DRIVING ACTIONS BEGIN*/
void CatchErrorOccuring(CanDataFrameInit *canFrame)
{
	/* BMS Errors handling */
	if( (canFrame->rx_header.StdId == 0x85) || (canFrame->rx_header.StdId == 0x90) || (canFrame->rx_header.StdId == 0x95) )
	{
		error = true;
		StopCanCommunication();
	}
}

void WarningHandler(CanDataFrameInit *canFrame)
{
	if(canFrame->rx_header.StdId == 0x55)
	{
		SendErrorFrame(canFrame->rx_data[1], canFrame->rx_data[2]);
	}
}

void SendErrorFrame(uint8_t highCondition, uint8_t lowCondition)
{
	if( (highCondition >= 0x01) && (lowCondition>=0xD0) )
	{
		UsbTransferDataByte(0x56, 0x01, 0x01, 0, 0, 0, 0, 0, 0);
	}
	else
	{
		UsbTransferDataByte(0x56, 0x0, 0x0, 0, 0, 0, 0, 0, 0);
	}
}


/*DRIVING ACTIONS END*/
/*CAN COMMUNICATION SECTION END*/

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
