/**
  ******************************************************************************
  * File Name          : canopen_object_dict.c
  * Description        : Zbior obiektow znajdujacych sie w sieci. Parametry dobrane
  *  					 wg standardu CANopen.
  ******************************************************************************
  */


#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include "main.h"
//#include "gpio.h"

/*******************************************************************************
 CANopen DATA DYPES
 *******************************************************************************/
typedef unsigned char bool_t; /**< bool_t */
typedef float float32_t; /**< float32_t */
typedef long double float64_t; /**< float64_t */
typedef char char_t; /**< char_t */
typedef unsigned char oChar_t; /**< oChar_t */
typedef unsigned char domain_t; /**< domain_t */

typedef uint8_t UNSIGNED8;
typedef uint16_t UNSIGNED16;
typedef uint32_t UNSIGNED32;
typedef uint64_t UNSIGNED64;
typedef int8_t INTEGER8;
typedef int16_t INTEGER16;
typedef int32_t INTEGER32;
typedef int64_t INTEGER64;
typedef float32_t REAL32;
typedef float64_t REAL64;
typedef char_t VISIBLE_STRING;
typedef oChar_t OCTET_STRING;
typedef domain_t DOMAIN;


/*******************************************************************************
 DEVICE INFO:
 VendorName:     STMicroelectronics
 ProductName:    STM32F407VGT6
 *******************************************************************************/

/*******************************************************************************
 FEATURES
 *******************************************************************************/
#define CO_NO_SYNC                  0   //Associated objects:
#define CO_NO_EMERGENCY             0   //Associated objects:
#define CO_NO_SDO_SERVER            0   //Associated objects:
#define CO_NO_SDO_CLIENT            0
#define CO_NO_RPDO                  0   //Associated objects:
#define CO_NO_TPDO                  0   //Associated objects:
#define CO_NO_NMT_MASTER            0
#define CO_NO_TRACE                 0
#define SYNC                 		100 //SYNC value in miliseconds
#define SDO_Receive_Byte0			0b00101111 //Using SDO protocol results in using this value as its first value when receiving data
#define SDO_Send_Byte0				0b0100 //Using SDO protocol results in using this value as its first value when sending data


//typedef struct {
//	uint8_t tx_data[8];
//	CAN_TxHeaderTypeDef tx_header;
//	CAN_RxHeaderTypeDef rx_header;
//	uint8_t rx_data[8];
//} CanDataFrameInit;

typedef struct CanFrameBase
{
	//Frame StdId - ID of individual frame;
	UNSIGNED32 ID;
	UNSIGNED32 DLC;
	UNSIGNED8 AdditionalData;
} CanFrameBase;

typedef struct
{
	//Data storage
	UNSIGNED32 node_id;

	//Index of node
	UNSIGNED16 index;
	UNSIGNED8 subindex_1;
	UNSIGNED8 subindex_2;
	UNSIGNED8 subindex_3;

	//ID of SDO protocol that we are using to send data
	UNSIGNED32 sdo_upload_id;

	//ID of SDO protocol used to receive data
	UNSIGNED32 sdo_download_id;

	//ID of PDO protocol used to receive data
	UNSIGNED32 pdo_consumer_id;

	//ID of PDO protocol that we are using to send data
	UNSIGNED32 pdo_producer_id;

	//Emergency ID used to send data about ongoing emergency of node
	UNSIGNED8 emcy_id;
} CanopenNode;

typedef struct Dashboard
{
	/*Sending messages from ECM perspective*/

	//TSDO Upload: Lights (0x01 - postojowe; 0x02 - dzienne; 0x03 - długie; 0x04 - przeciwmgielne; 0x05 - kierunkowskaz lewy; 0x06 - kierunkowskaz prawy; 0x07 - awaryjne) Other (0x01 - pasy bezpieczeństwa; 0x02 - otwarte drzwi; 0x03) EMCY (Contains device ID and error code)
	//Receiver: Dashboard, Lights controller
	//Sender: ECM
	CanFrameBase LightsSDO;

	//TPDO: Output power in kW (bytes 0-1); Battery charge level (bytes 2-3); Average temperature (bytes 4-6)
	//Receiver: Dashboard
	//Sender: ECM
	CanFrameBase BMSInformationFramePDO;

	//TPDO: Consumed power in kW (bytes 0-3) & Vehicle speed (bytes 4-7)
	//Receiver: Dashboard
	//Sender: ECM
	CanFrameBase Inverter1ConsumedPowerPDO;

	//TPDO: Consumed power in kW (bytes 0-3) & Vehicle speed (bytes 4-7)
	//Receiver: Dashboard
	//Sender: ECM
	CanFrameBase Inverter2ConsumedPowerPDO;
} Dashboard;

typedef struct BMS
{
	/*Receiving Messages from ECM perspective*/
	//EMCY: DATA0 contains error information from 0x0 - 0x5. 0 - over voltage; 1 - undervoltage; 2 - over current; 3 - over temperature; 4 - under temperature; 5 - MPPTs powered off info
	//Receiver: EMC
	//Sender: BMS
	CanFrameBase ErrorFrameEMCY;

	//TPDO: 1-4 of 29 battery voltages. Each voltage consists of two data bytes
	//Receiver: EMC
	//Sender: BMS
	CanFrameBase BatteryVoltageFrame1_4PDO;
	//TPDO: 5-8 of 29 battery voltages. Each voltage consists of two data bytes
	//Receiver: EMC
	//Sender: BMS
	CanFrameBase BatteryVoltageFrame5_8PDO;

	//TPDO: 9-12 of 29 battery voltages. Each voltage consists of two data bytes
	//Receiver: EMC
	//Sender: BMS
	CanFrameBase BatteryVoltageFrame9_12PDO;

	//TPDO: 13-16 of 29 battery voltages. Each voltage consists of two data bytes
	//Receiver: EMC
	//Sender: BMS
	CanFrameBase BatteryVoltageFrame13_16PDO;

	//TPDO: 17-20 of 29 battery voltages. Each voltage consists of two data bytes
	//Receiver: EMC
	//Sender: BMS
	CanFrameBase BatteryVoltageFrame17_20PDO;

	//TPDO: 21-24 of 29 battery voltages. Each voltage consists of two data bytes
	//Receiver: EMC
	//Sender: BMS
	CanFrameBase BatteryVoltageFrame21_24PDO;

	//TPDO: 25-28 of 29 battery voltages. Each voltage consists of two data bytes
	//Receiver: EMC
	//Sender: BMS
	CanFrameBase BatteryVoltageFrame25_28PDO;

	//TPDO: 29 of 29 battery voltages. Each voltage consists of two data bytes. 2 B - 4 B declare battery current
	//Receiver: EMC
	//Sender: BMS
	CanFrameBase BatteryVoltageFrame29PDO;

	//TPDO: 1-4 of 12 battery temperatures. Each temperature consists of two data bytes
	//Receiver: EMC
	//Sender: BMS
	CanFrameBase BatteryTemperatur1_4PDO;

	//TPDO: 5-8 of 12 battery temperatures. Each temperature consists of two data bytes
	//Receiver: EMC
	//Sender: BMS
	CanFrameBase BatteryTemperatur5_8PDO;

	//TPDO: 9-12 of 12 battery temperatures. Each temperature consists of two data bytes
	//Receiver: EMC
	//Sender: BMS
	CanFrameBase BatteryTemperatur9_12PDO;

	//TPDO: Output power in kW (bytes 0-1); Battery charge level (bytes 2-3); Average temperature (bytes 4-6)
	//Receiver: EMC
	//Sender: BMS
	CanFrameBase OutputPowerInkW;
} BMS;

typedef struct Inverter
{
	/*Receiving Messages from ECM perspective*/
	//EMCY: DATA0 contains error information.
	//Sender: Inverter
	CanFrameBase ErrorFrameEMCY;

	//TPDO: Inverter temperature (0-3 B) & Engine temperature (4-7 B)
	//Receiver: EMC
	//Sender: Inverter
	CanFrameBase TemperatureFrameTPDO;

	//TPDO: Phase current no. 1
	//Receiver: EMC
	//Sender: Inverter
	CanFrameBase Phase1CurrentFrameTPDO;

	//TPDO: Phase current no. 2
	//Receiver: EMC
	//Sender: Inverter
	CanFrameBase Phase2CurrentFrameTPDO;

	//TPDO: Phase current no. 3
	//Receiver: EMC
	//Sender: Inverter
	CanFrameBase Phase3CurrentFrameTPDO;

	//TPDO: Torque (0-3 B) & Rotation Speed (4-7 B)
	//Receiver: EMC
	//Sender: Inverter
	CanFrameBase RotationSpeedTPDO;

	//TPDO: DC bus voltage
	//Receiver: EMC
	//Sender: Inverter
	CanFrameBase DCBusVoltage;

	//TPDO: Consumed power in kW (bytes 0-3) & Vehicle speed (bytes 4-7)
	//Receiver: EMC
	//Sender: Inverter
	CanFrameBase ConsumedPowerInkW;
} Inverter;

typedef struct MPPT
{
	UNSIGNED8 Data[8];
	/*Receiving Messages from ECM perspective*/

	//RSDO Upload: prąd i napięcie wejściowe
	//Receiver: ECM
	//Sender: MPPT
	CanFrameBase ReceivedCurrentAndVoltage1SDO;

	//RSDO Upload: prąd i napięcie wyjściowe
	//Receiver: ECM
	//Sender: MPPT
	CanFrameBase ReceivedCurrentAndVoltage2SDO;

	//RSDO Upload: tryb pracy
	//Receiver: ECM
	//Sender: MPPT
	CanFrameBase ReceivedOperationalStateSDO;


	/*Sending messages from ECM perspective*/

	//RSDO Upload: prąd i napięcie wejściowe
	//Receiver: MPPT
	//Sender: ECM
	CanFrameBase SendCurrentAndVoltage1SDO;

	//RSDO Upload: prąd i napięcie wyjściowe
	//Receiver: MPPT
	//Sender: ECM
	CanFrameBase SendCurrentAndVoltage2SDO;

	//RSDO Upload: tryb pracy
	//Receiver: MPPT
	//Sender: ECM
	CanFrameBase SendOperationalStateSDO;
} MPPT;

typedef struct LightsController
{
	//TSDO Upload: Lights (0x01 - postojowe; 0x02 - dzienne; 0x03 - długie; 0x04 - przeciwmgielne; 0x05 - kierunkowskaz lewy; 0x06 - kierunkowskaz prawy; 0x07 - awaryjne) Other (0x01 - pasy bezpieczeństwa; 0x02 - otwarte drzwi; 0x03) EMCY (Contains device ID and error code)
	CanFrameBase LightsData;
} LightsController;


CanFrameBase CanDataFrameBase(UNSIGNED32 _ID, UNSIGNED32 _DLC);

CanFrameBase CanDataFrameBaseWithAdditionalData(UNSIGNED32 _ID, UNSIGNED32 _DLC, UNSIGNED8 AddData);

void MakeDataFrame(MPPT *MPPT, UNSIGNED8 byte0, UNSIGNED8 byte1, UNSIGNED8 byte2,
					UNSIGNED8 byte3, UNSIGNED8 byte4, UNSIGNED8 byte5, UNSIGNED8 byte6, UNSIGNED8 byte7);

/*******************************************************************************
 OBJECT DICTIONARY
 *******************************************************************************/
//#define CO_OD_NoOfElements             9
////CanopenNode dashboard;				// ECM
////CanopenNode inverter_1;				// ECM
////CanopenNode inverter_2;				// ECM
////CanopenNode mppt_1;					// ECM
////CanopenNode mppt_2;					// ECM
////CanopenNode mppt_3;					// ECM
////CanopenNode ecm;
////CanopenNode bms;	                // MPPTs, Inverters, Dashboard, Lights controller
CanopenNode lights_controller;		// ECM
//					// ECM
#define CO_OD_NoOfElements  10
Dashboard _Dashboard;
BMS _BMS;
Inverter _Inverter1;
Inverter _Inverter2;
MPPT _MPPT1;
MPPT _MPPT2;
MPPT _MPPT3;
LightsController _LightsController;
UNSIGNED8 DefaultFFCanFrame[8];
void CanopenObjectDictInit();
void CanOpenObjectsInit();


