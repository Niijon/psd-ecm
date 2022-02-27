/**
  ******************************************************************************
  * File Name          : CAN.h
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __can_H
#define __can_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include <stdio.h>
/* USER CODE END Includes */

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

/* USER CODE BEGIN Private defines */

/*******************************************************************************
 DEFINES
 *******************************************************************************/
#define CAN_HIGH_SPEED hcan1
#define CAN_LOW_SPEED hcan2

#define OPERATIONAL_STATE		0x01
#define STOPPED_STATE			0x02
#define PRE_OPERATIONAL_STATE	0x80
#define RESET_APPLICATION		0x81
#define RESET_COMMUNICATION		0x82

#define SDO_UPLOAD   0b00100000
#define SDO_DOWNLOAD 0b01000000

extern bool error;
extern bool charging;
extern bool highVoltageActive;

/*******************************************************************************
 DECLARATIONS
 *******************************************************************************/
typedef struct CanDataFrameInit{
	uint8_t tx_data[8];
	CAN_TxHeaderTypeDef tx_header;
	CAN_RxHeaderTypeDef rx_header;
	uint8_t rx_data[8];
} CanDataFrameInit;

typedef struct Charger{
	uint8_t voltage_higher_byte;
	uint8_t voltage_lower_byte;
	uint8_t current_higher_byte;
	uint8_t current_lower_byte;
	uint16_t max_current;
	uint16_t max_voltage;
	uint32_t can_charger_id;
	uint32_t can_receiving_id;
}Charger;

#define byteMaxValue 256

/*Mini math lib declarations*/
typedef struct GluedBytes
{
	uint8_t lowerByte;
	uint8_t higherByte;
} GluedBytes;

GluedBytes Parse2Bytes(int toParse);
int unParse2Bytes(uint8_t lowerByte, uint8_t higherByte);
/**/

void CatchErrorOccuring(CanDataFrameInit *canFrame);
void CatchChargingErrorOccuring(CanDataFrameInit *canFrame);
void SendErrorFrame(uint8_t highCondition, uint8_t lowCondition);
void WarningHandler(CanDataFrameInit *canFrame);

extern CanDataFrameInit can_frame_template;
extern CanDataFrameInit can_rx_frame_template;


/* can peripherial initialization */
void CanInit(CAN_HandleTypeDef hcanx);
void CanConfigFilter(CAN_HandleTypeDef hcanx, uint8_t can_filter_bank);

void CanReceivedData(CAN_HandleTypeDef chosen_network, CanDataFrameInit *ptr_can_rx_frame_template);
CanDataFrameInit CanSaveReceivedData(CAN_HandleTypeDef chosen_network, CanDataFrameInit *ptr_can_rx_frame_template);
/*void CanTransfer(CAN_HandleTypeDef hcanx, CanDataFrameInit *canFrame, uint32_t sender_id);*/

void CanClearTxDataFrame(CanDataFrameInit *can_frame_template);
void CanClearRxDataFrame(CanDataFrameInit *ptr_can_frame_template);
void CanTransferFrame(CAN_HandleTypeDef chosen_network, CanDataFrameInit *ptr_can_frame_template);

/* canopen services */
void CanSendSync(CAN_HandleTypeDef hcanx, CanDataFrameInit *can_frame_template);
void CanSendNmt(CAN_HandleTypeDef hcanx, uint8_t state, uint8_t node_id,
		CanDataFrameInit *can_frame_template);
void CanSendPdo(CAN_HandleTypeDef chosen_network, uint32_t frame_pdo_id,
		uint8_t number_of_bytes, CanDataFrameInit *can_frame_template,
		uint8_t byte0, uint8_t byte1, uint8_t byte2, uint8_t byte3,
		uint8_t byte4, uint8_t byte5, uint8_t byte6, uint8_t byte7);
void CanSendSdo(CAN_HandleTypeDef chosen_network, uint32_t frame_sdo_id,
		CanDataFrameInit *ptr_can_frame_template, uint8_t number_of_bytes,
		uint8_t command_byte, uint8_t byte0, uint8_t byte1, uint8_t byte2,
		uint8_t byte3, uint8_t byte4, uint8_t byte5, uint8_t byte6);

void CanSendExtendedIdMessage(CAN_HandleTypeDef chosen_network,
		CanDataFrameInit *ptr_can_frame_template, uint32_t FrameId, uint8_t DLC,
	uint8_t byte0, uint8_t byte1, uint8_t byte2, uint8_t byte3,
	uint8_t byte4, uint8_t byte5, uint8_t byte6, uint8_t byte7);

CanDataFrameInit CanMakeFrameWithValue(CanDataFrameInit *CanFrame, uint8_t frameId, uint8_t byte0, uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, uint8_t byte5, uint8_t byte6, uint8_t byte7);

void StartCanCommunication();
void StopCanCommunication();
/* USER CODE END Private defines */

void MX_CAN1_Init(void);
void MX_CAN2_Init(void);

/* USER CODE BEGIN Prototypes */
void ActUponCurrentAndVoltage(CanDataFrameInit *canFrame, int maxVoltage, int maxCurrent);
void ChargingStateModule();
void InitCharger(uint16_t charging_voltage, uint16_t charging_current);
void DrivingStateModule();
void BMSWarningHandler(CanDataFrameInit *canFrame);
/*Only optional to talk out with people*/
void ParkingStateModule();
void HandleHighSpeed();
void HandleLowSpeed();
void StartCharging();
void ReverseManagement(CanDataFrameInit *canFrame);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ can_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
