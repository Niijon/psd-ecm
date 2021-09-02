/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : ECM Module software
 * @author		   : Szymon Kacperek
 * @mail           : szymonkacperek96@gmail.com
 * @date		   : 14/08/2020
 ******************************************************************************
 * Code created by me is signed in longer * arrays. Naming is adapted to Google C++ Style Guide.
 *
 ******************************************************************************
 * NOTES
 * @ 24/11/2020 Perspectives to develop:
 * - [x] clear every message after being sent in order to CAN data sent over USB be clear;
 * - [ ] create a ENUM type which would return CAN peripherial state after calling CAN functions;
 * - [ ] allocate memory dynamically in order to build usb data frames;
 * - [ ] sent usb data in 200-elements array with specified time interval e.g. 30 or 60 Hz;
 *
 *
 *
 *
 *
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "can.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/************************************************************************************************
 PRIVATE INCLUDES
 ************************************************************************************************/
#include "canopen_object_dict.h"
#include "usbd_cdc_if.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
CanDataFrameInit canFrame;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/************************************************************************************************
 PRIVATE VARIABLES
 ************************************************************************************************/

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CAN1_Init();
  MX_CAN2_Init();
  MX_TIM10_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */

	/************************************************************************************************
	 CAN PERIPHERIAL AND ITS COMPONENTS
	 ************************************************************************************************/
	CanopenObjectDictInit();
	CanConfigFilter(CAN_HIGH_SPEED, 1, 0x0000, 0x0000, 0x0000, 0x0000);
	CanInit(CAN_HIGH_SPEED);

	CanConfigFilter(CAN_LOW_SPEED, 0, 0x0000, 0x0000, 0x0000, 0x0000);
	CanInit(CAN_LOW_SPEED);

	HAL_TIM_Base_Start_IT(&htim10);

	/************************************************************************************************
	 TURNING ON THE MODULES
	 ************************************************************************************************/
	CanSendNmt(CAN_HIGH_SPEED, OPERATIONAL_STATE, bms.node_id,
			&can_frame_template);
	CanSendNmt(CAN_HIGH_SPEED, OPERATIONAL_STATE, inverter_1.node_id,
			&can_frame_template);
	CanSendNmt(CAN_HIGH_SPEED, OPERATIONAL_STATE, inverter_2.node_id,
			&can_frame_template);
	CanSendNmt(CAN_HIGH_SPEED, OPERATIONAL_STATE, mppt_1.node_id,
			&can_frame_template);
	CanSendNmt(CAN_HIGH_SPEED, OPERATIONAL_STATE, mppt_2.node_id,
			&can_frame_template);
	CanSendNmt(CAN_HIGH_SPEED, OPERATIONAL_STATE, mppt_3.node_id,
			&can_frame_template);
	CanSendNmt(CAN_HIGH_SPEED, OPERATIONAL_STATE, lights_controller.node_id,
			&can_frame_template);
	CanSendNmt(CAN_HIGH_SPEED, OPERATIONAL_STATE, dashboard.node_id,
			&can_frame_template);
	/************************************************************************************************
	 USB
	 ************************************************************************************************/
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	error = false;
	charging = false;
	driving = false;
	UNSIGNED8 speed = 0;
	UNSIGNED8 inc =5;
	while (1) {
//		ChargingStateModule();
//		DrivingStateModule();
//		//initializing data upload
//		CanSendSdo(CAN_HIGH_SPEED, inverter_1.node_id , &can_frame_template , 0, 1, 0, 0, 0, 0, 0, 0, 0);
//		//actual data upload.
//		CanSendSdo(CAN_HIGH_SPEED, bms.sdo_upload_id, &can_frame_template , 2, 2, 0, 0, 0, 0, 0, 0, 0);
//		UsbTransfer(&can_frame_template);

		if(speed>100)
			inc = -inc;
		else if(speed<25)
			inc = -inc;

		UsbTransferDataByte(0x290, 0, 0, 0, 0x45, 0, 0, 0, 0);
		HAL_Delay(100);

//		CanSendSdo(CAN_LOW_SPEED, 0x581, &can_frame_template, 8, SDO_DOWNLOAD, 0x6, 0x9, 0, 0, 0, 0, 0);
//		HAL_Delay(1000);

		UsbTransferDataByte(0x581, SDO_DOWNLOAD, 0x6, 0x1, 0, 0, 0, 0, 0);
		HAL_Delay(100);

		UsbTransferDataByte(0x185, 0xD2, 0xD2, 0xD0, 0xD3, 0xD2, 0xD0, 0xBE, 0xD0);
		HAL_Delay(100);
		/* Extended Can frame method */
//		CanSendExtendedIdMessage(hcan1, &can_frame_template, 10, 8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);



    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 120;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV6;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/*CAR STATES MODULES*/
void ChargingStateModule()
{
	StopCanCommunication();
	HAL_Delay(2);
	StartCanCommunication();
	while(charging && !error)
	{
		canFrame = CanSaveReceivedData(hcan1, &can_frame_template);
		CanClearRxDataFrame(&can_frame_template);
		if(canFrame.rx_header.ExtId == 0x18FF50E5)
		{
			if(ActUponCurrentAndVoltage(&canFrame, 3000, 200))
			{
				CanSendExtendedIdMessage(hcan1, &can_frame_template, 10, 8, 0x03, 0xE8, 0, 10, 0, 0, 0, 0);
				HAL_Delay(1000);
			}
			CanClearRxDataFrame(&canFrame);
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

}

void HandleHighSpeed()
{

	canFrame = CanSaveReceivedData(CAN_HIGH_SPEED, &can_frame_template);
	UsbTransfer(&canFrame);
	error = CatchErrorOccuring(&canFrame);
	CanClearRxDataFrame(&can_frame_template);
	CanClearRxDataFrame(&canFrame);
}

void HandleLowSpeed()
{
	canFrame = CanSaveReceivedData(CAN_LOW_SPEED, &can_frame_template);
	UsbTransfer(&canFrame);
	error = CatchErrorOccuring(&canFrame);
	CanClearRxDataFrame(&can_frame_template);
	CanClearRxDataFrame(&canFrame);
}

/*Only optional to talk out with people*/
void ParkingStateModule()
{

}



/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}



#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
