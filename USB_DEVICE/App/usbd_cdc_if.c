/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : usbd_cdc_if.c
 * @version        : v1.0_Cube
 * @brief          : Usb device for Virtual Com Port.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc_if.h"
#include "gpio.h"

/* USER CODE BEGIN INCLUDE */
/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
static uint8_t lineCoding[7] // 115200bps, 1stop, no parity, 8bit
= { 0x00, 0xC2, 0x01, 0x00, 0x00, 0x00, 0x08 };
uint8_t message_length = 0;
uint8_t uart_data_to_send[33];
uint8_t usb_received_data[33];
uint8_t buffer_size = 0;
uint8_t usb_tx_data_buffer[23];

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device library.
  * @{
  */

/** @addtogroup USBD_CDC_IF
  * @{
  */

/** @defgroup USBD_CDC_IF_Private_TypesDefinitions USBD_CDC_IF_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Defines USBD_CDC_IF_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */
/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Macros USBD_CDC_IF_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Variables USBD_CDC_IF_Private_Variables
  * @brief Private variables.
  * @{
  */

/* Create buffer for reception and transmission           */
/* It's up to user to redefine and/or remove those define */
/** Received data over USB are stored in this buffer      */
uint8_t UserRxBufferHS[APP_RX_DATA_SIZE];

/** Data to send over USB CDC are stored in this buffer   */
uint8_t UserTxBufferHS[APP_TX_DATA_SIZE];

/* USER CODE BEGIN PRIVATE_VARIABLES */

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Exported_Variables USBD_CDC_IF_Exported_Variables
  * @brief Public variables.
  * @{
  */

extern USBD_HandleTypeDef hUsbDeviceHS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_FunctionPrototypes USBD_CDC_IF_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t CDC_Init_HS(void);
static int8_t CDC_DeInit_HS(void);
static int8_t CDC_Control_HS(uint8_t cmd, uint8_t* pbuf, uint16_t length);
static int8_t CDC_Receive_HS(uint8_t* pbuf, uint32_t *Len);
static int8_t CDC_TransmitCplt_HS(uint8_t *pbuf, uint32_t *Len, uint8_t epnum);

/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */

/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */

USBD_CDC_ItfTypeDef USBD_Interface_fops_HS =
{
  CDC_Init_HS,
  CDC_DeInit_HS,
  CDC_Control_HS,
  CDC_Receive_HS,
  CDC_TransmitCplt_HS
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the CDC media low layer over the USB HS IP
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Init_HS(void)
{
  /* USER CODE BEGIN 8 */
	/* Set Application Buffers */
	USBD_CDC_SetTxBuffer(&hUsbDeviceHS, UserTxBufferHS, 0);
	USBD_CDC_SetRxBuffer(&hUsbDeviceHS, UserRxBufferHS);
	return (USBD_OK);
  /* USER CODE END 8 */
}

/**
  * @brief  DeInitializes the CDC media low layer
  * @param  None
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_DeInit_HS(void)
{
  /* USER CODE BEGIN 9 */
	return (USBD_OK);
  /* USER CODE END 9 */
}

/**
  * @brief  Manage the CDC class requests
  * @param  cmd: Command code
  * @param  pbuf: Buffer containing command data (request parameters)
  * @param  length: Number of data to be sent (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Control_HS(uint8_t cmd, uint8_t* pbuf, uint16_t length)
{
  /* USER CODE BEGIN 10 */
	switch (cmd) {
	case CDC_SEND_ENCAPSULATED_COMMAND:

		break;

	case CDC_GET_ENCAPSULATED_RESPONSE:

		break;

	case CDC_SET_COMM_FEATURE:

		break;

	case CDC_GET_COMM_FEATURE:

		break;

	case CDC_CLEAR_COMM_FEATURE:

		break;

		/*******************************************************************************/
		/* Line Coding Structure                                                       */
		/*-----------------------------------------------------------------------------*/
		/* Offset | Field       | Size | Value  | Description                          */
		/* 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits per second*/
		/* 4      | bCharFormat |   1  | Number | Stop bits                            */
		/*                                        0 - 1 Stop bit                       */
		/*                                        1 - 1.5 Stop bits                    */
		/*                                        2 - 2 Stop bits                      */
		/* 5      | bParityType |  1   | Number | Parity                               */
		/*                                        0 - None                             */
		/*                                        1 - Odd                              */
		/*                                        2 - Even                             */
		/*                                        3 - Mark                             */
		/*                                        4 - Space                            */
		/* 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or 16).          */
		/*******************************************************************************/

	case CDC_SET_LINE_CODING:
		memcpy(lineCoding, pbuf, sizeof(lineCoding));
		break;

	case CDC_GET_LINE_CODING:
		memcpy(pbuf, lineCoding, sizeof(lineCoding));
		break;

	case CDC_SET_CONTROL_LINE_STATE:

		break;

	case CDC_SEND_BREAK:

		break;

	default:
		break;
	}

	return (USBD_OK);
  /* USER CODE END 10 */
}

/**
  * @brief Data received over USB OUT endpoint are sent over CDC interface
  *         through this function.
  *
  *         @note
  *         This function will issue a NAK packet on any OUT packet received on
  *         USB endpoint until exiting this function. If you exit this function
  *         before transfer is complete on CDC interface (ie. using DMA controller)
  *         it will result in receiving more data while previous ones are still
  *         not sent.
  *
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAILL
  */
static int8_t CDC_Receive_HS(uint8_t* Buf, uint32_t *Len)
{
  /* USER CODE BEGIN 11 */
	USBD_CDC_SetRxBuffer(&hUsbDeviceHS, &Buf[0]);
	USBD_CDC_ReceivePacket(&hUsbDeviceHS);
	strlcpy((&usb_received_data), Buf, *Len + 1);

	if (usb_received_data[0] == '1') {
		HAL_GPIO_WritePin(LED_D4_GPIO_Port, LED_D4_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LED_D4_GPIO_Port, LED_D4_Pin, GPIO_PIN_RESET);
	}
//	CDC_ReceiveCallback(&Buf, Len[0]);
	return (USBD_OK);
  /* USER CODE END 11 */
}

/**
  * @brief  Data to send over USB IN endpoint are sent over CDC interface
  *         through this function.
  * @param  Buf: Buffer of data to be sent
  * @param  Len: Number of data to be sent (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY
  */
uint8_t CDC_Transmit_HS(uint8_t* Buf, uint16_t Len)
{
  uint8_t result = USBD_OK;
  /* USER CODE BEGIN 12 */
	USBD_CDC_HandleTypeDef *hcdc =
			(USBD_CDC_HandleTypeDef*) hUsbDeviceHS.pClassData;
	if (hcdc->TxState != 0) {
		return USBD_BUSY;
	}
	USBD_CDC_SetTxBuffer(&hUsbDeviceHS, Buf, Len);
	result = USBD_CDC_TransmitPacket(&hUsbDeviceHS);
  /* USER CODE END 12 */
  return result;
}

/**
  * @brief  CDC_TransmitCplt_HS
  *         Data transmited callback
  *
  *         @note
  *         This function is IN transfer complete callback used to inform user that
  *         the submitted Data is successfully sent over USB.
  *
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_TransmitCplt_HS(uint8_t *Buf, uint32_t *Len, uint8_t epnum)
{
  uint8_t result = USBD_OK;
  /* USER CODE BEGIN 14 */
	UNUSED(Buf);
	UNUSED(Len);
	UNUSED(epnum);
  /* USER CODE END 14 */
  return result;
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
 * @brief: transfer sent or received can data frame over virtual com port
 * @param ptr_can_frame_template
 *
 **/

void UsbTransfer(CanDataFrameInit *ptr_can_frame_template) {
//	uint8_t buffer_size = ptr_can_frame_template->rx_header.DLC * 2 + 7;
//	uint8_t usb_tx_data_buffer[buffer_size];

	message_length = sprintf(&usb_tx_data_buffer,
			"%03X[%01X]%02X%02X%02X%02X%02X%02X\r\n", // 050[8]DEADBEEFFEEDDEAD
			ptr_can_frame_template->rx_header.StdId,
			ptr_can_frame_template->rx_header.DLC,
			ptr_can_frame_template->rx_data[0],
			ptr_can_frame_template->rx_data[1],
			ptr_can_frame_template->rx_data[2],
			ptr_can_frame_template->rx_data[3],
			ptr_can_frame_template->rx_data[4],
			ptr_can_frame_template->rx_data[5]);
	CDC_Transmit_HS(usb_tx_data_buffer, message_length);

}

void UsbTransferData(uint8_t node_id, CanDataFrameInit *CanFrameTemplate) {
	message_length = sprintf(uart_data_to_send,
			"USB: %02X%02X%02X%02X%02X%02X%02X%02X\r", node_id,
			CanFrameTemplate->tx_data[0], CanFrameTemplate->tx_data[1],
			CanFrameTemplate->tx_data[2], CanFrameTemplate->tx_data[3],
			CanFrameTemplate->tx_data[4], CanFrameTemplate->tx_data[5],
			CanFrameTemplate->tx_data[6], CanFrameTemplate->tx_data[7]);

	CDC_Transmit_HS(&uart_data_to_send, message_length);

}

void UsbSendData(uint8_t node_id,uint8_t byte0, uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, uint8_t byte5, uint8_t byte6)
{

	message_length = sprintf(uart_data_to_send,
			"USB: [%02X]%02X:%02X:%02X:%02X:%02X:%02X:%02X\r",
			node_id, byte0, byte1, byte2, byte3, byte4, byte5, byte6);

	CDC_Transmit_HS(&uart_data_to_send, message_length);
}

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
