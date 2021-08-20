/**
  ******************************************************************************
  * File Name          : canopen_object_dict.c
  * Description        : Zbior danych obiektow znajdujacych sie w sieci wg
  * 					 standardu CANopen
  ******************************************************************************
  */

#include <canopen_object_dict.h>

//While making SDO frame 0 byte must be SDO_Receive_Byte0, when receiving data and SDO_Send_Byte0 when sending data
//CanDataFrameRxMessage MakeCanDataFrameRxMessage
//		(UNSIGNED8 byte0, UNSIGNED8 byte1,  UNSIGNED8 byte2,  UNSIGNED8 byte3, UNSIGNED8 byte4,
//		UNSIGNED8 byte5,  UNSIGNED8 byte6,  UNSIGNED8 byte7, UNSIGNED32 DLC,UNSIGNED32 StdId){
//	CanDataFrameRxMessage CDFR;
//	CDFR.data[0] = byte0;
//	CDFR.data[1] = byte1;
//	CDFR.data[2] = byte2;
//	CDFR.data[3] = byte3;
//	CDFR.data[4] = byte4;
//	CDFR.data[5] = byte5;
//	CDFR.data[6] = byte6;
//	CDFR.data[7] = byte7;
//	CDFR.rx_header.DLC = DLC;
//	CDFR.rx_header.IDE = 0x00000000U;
//	CDFR.rx_header.RTR = 0x00000000U;
//	CDFR.rx_header.StdId = StdId;
//	CDFR.rx_header.Timestamp = DISABLE;
//	return CDFR;
//}

//CanDataFrameTxMessage MakeCanDataFrameTxMessage
//						(UNSIGNED8 byte0, UNSIGNED8 byte1,  UNSIGNED8 byte2,  UNSIGNED8 byte3,
//						UNSIGNED8 byte4,  UNSIGNED8 byte5,  UNSIGNED8 byte6,  UNSIGNED8 byte7,
//						UNSIGNED32 DLC, UNSIGNED32 StdId){
//	CanDataFrameTxMessage CDFT;
//	CDFT.data[0] = byte0;
//	CDFT.data[1] = byte1;
//	CDFT.data[2] = byte2;
//	CDFT.data[3] = byte3;
//	CDFT.data[4] = byte4;
//	CDFT.data[5] = byte5;
//	CDFT.data[6] = byte6;
//	CDFT.data[7] = byte7;
//	CDFT.tx_header.DLC = DLC;
//	CDFT.tx_header.IDE = 0x00000000U;
//	CDFT.tx_header.RTR = 0x00000000U;
//	CDFT.tx_header.StdId = StdId;
//	CDFT.tx_header.TransmitGlobalTime = DISABLE;
//	return CDFT;
//}

CanFrameBase CanDataFrameBase(UNSIGNED32 _ID, UNSIGNED32 _DLC)
{
	CanFrameBase CFB;
	CFB.ID = _ID;
	CFB.DLC = _DLC;
	CFB.AdditionalData = 0;
	return CFB;
}

CanFrameBase CanDataFrameBaseWithAdditionalData(UNSIGNED32 _ID, UNSIGNED32 _DLC, UNSIGNED8 AddData)
{
	CanFrameBase CFB;
	CFB.ID = _ID;
	CFB.DLC = _DLC;
	CFB.AdditionalData = AddData;
	return CFB;
}

void MakeDataFrame(MPPT *MPPT, UNSIGNED8 byte0, UNSIGNED8 byte1, UNSIGNED8 byte2,
					UNSIGNED8 byte3, UNSIGNED8 byte4, UNSIGNED8 byte5, UNSIGNED8 byte6, UNSIGNED8 byte7)
{
	MPPT->Data[0] = byte0;
	MPPT->Data[1] = byte1;
	MPPT->Data[2] = byte2;
	MPPT->Data[3] = byte3;
	MPPT->Data[4] = byte4;
	MPPT->Data[5] = byte5;
	MPPT->Data[6] = byte6;
	MPPT->Data[7] = byte7;
}


void CanopenObjectDictInit()
{
	dashboard.node_id = 0x70;
	dashboard.sdo_download_id = 0x581;		// Channel with ecm
	dashboard.emcy_id = 0xF0;

	inverter_1.node_id = 0x10;
	inverter_1.pdo_producer_id = 0x290;	// 0x290 - 0x296 Channel with ecm
	inverter_1.emcy_id = 0x90;

	inverter_2.node_id = 0x15;
	inverter_2.pdo_producer_id = 0x395;	// 0x395 - 0x401 Channel with ecm
	inverter_2.emcy_id = 0x95;

	mppt_1.node_id = 0x33;
	mppt_1.sdo_upload_id = 0x633;		// Channel with ecm
	mppt_1.sdo_download_id = 0x5B3;
	mppt_1.emcy_id = 0xB3;
	mppt_1.index = 0xAAAB;
	mppt_1.subindex_1 = 0x01;
	mppt_1.subindex_2 = 0x02;
	mppt_1.subindex_3 = 0x03;			// 0x01 - 0x03

	mppt_2.node_id = 0x36;
	mppt_2.sdo_upload_id = 0x636;		// Channel with ecm
	mppt_2.sdo_download_id = 0x5B6;
	mppt_2.emcy_id = 0xB6;
	mppt_2.index = 0xAAAA;
	mppt_2.subindex_1 = 0x01;
	mppt_2.subindex_2 = 0x02;
	mppt_2.subindex_3 = 0x03;			// 0x01 - 0x03

	mppt_3.node_id = 0x39;
	mppt_3.sdo_upload_id = 0x639;		// Channel with ecm
	mppt_3.sdo_download_id = 0x5B9;
	mppt_3.emcy_id = 0xB9;
	mppt_3.index = 0xAAAA;
	mppt_3.subindex_1 = 0x01;
	mppt_3.subindex_2 = 0x02;
	mppt_3.subindex_3 = 0x03; 			// 0x01 - 0x03

	ecm.node_id = 0x01;
	ecm.sdo_upload_id = 0x581;
	ecm.emcy_id = 0x81;

	bms.node_id = 0x05;
	bms.pdo_producer_id = 0x185;			// 0x185 - 0x196
	bms.emcy_id = 0x85;
	bms.sdo_download_id = 0x15;
	bms.sdo_upload_id = 0x69;

	lights_controller.node_id = 0x50;
	lights_controller.pdo_consumer_id = 0x581;
}
//
//void CanOpenObjectsInit()
//{
//	DefaultFFCanFrame[0] = 0xFF;
//	DefaultFFCanFrame[1] = 0xFF;
//	DefaultFFCanFrame[2] = 0xFF;
//	DefaultFFCanFrame[3] = 0xFF;
//	DefaultFFCanFrame[4] = 0xFF;
//	DefaultFFCanFrame[5] = 0xFF;
//	DefaultFFCanFrame[6] = 0xFF;
//	DefaultFFCanFrame[7] = 0xFF;
//
//	//BMS INIT BEGIN
//	_BMS.BatteryVoltageFrame1_4PDO = CanDataFrameBase(0x185, 8);
//	_BMS.BatteryVoltageFrame5_8PDO = CanDataFrameBase(0x186, 8);
//	_BMS.BatteryVoltageFrame9_12PDO = CanDataFrameBase(0x187, 8);
//	_BMS.BatteryVoltageFrame13_16PDO = CanDataFrameBase(0x188, 8);
//	_BMS.BatteryVoltageFrame17_20PDO = CanDataFrameBase(0x189, 8);
//	_BMS.BatteryVoltageFrame21_24PDO = CanDataFrameBase(0x18A, 8);
//	_BMS.BatteryVoltageFrame25_28PDO = CanDataFrameBase(0x18B, 8);
//	_BMS.BatteryVoltageFrame29PDO = CanDataFrameBase(0x18C, 5);
//	_BMS.BatteryTemperatur1_4PDO = CanDataFrameBase(0X18D, 8);
//	_BMS.BatteryTemperatur5_8PDO = CanDataFrameBase(0X18E, 8);
//	_BMS.BatteryTemperatur9_12PDO = CanDataFrameBase(0X18F, 8);
//	_BMS.OutputPowerInkW = CanDataFrameBase(0x190, 7);
//	/* BMS INIT END*/
//
//	/* Inverter1 INIT BEGIN*/
//	_Inverter1.TemperatureFrameTPDO = CanDataFrameBase(0x290,8);
//	_Inverter1.Phase1CurrentFrameTPDO = CanDataFrameBase(0x291,4);
//	_Inverter1.Phase2CurrentFrameTPDO = CanDataFrameBase(0x292,4);
//	_Inverter1.Phase3CurrentFrameTPDO = CanDataFrameBase(0x293,4);
//	_Inverter1.RotationSpeedTPDO = CanDataFrameBase(0x294,8);
//	_Inverter1.DCBusVoltage = CanDataFrameBase(0x295,4);
//	_Inverter1.ConsumedPowerInkW = CanDataFrameBase(0x296,8);
//	/* Inverter1 INIT END*/
//
//	/* Inverter2 INIT BEGIN*/
//	_Inverter2.TemperatureFrameTPDO = CanDataFrameBase(0x395,8);
//	_Inverter2.Phase1CurrentFrameTPDO = CanDataFrameBase(0x396,4);
//	_Inverter2.Phase2CurrentFrameTPDO = CanDataFrameBase(0x397,4);
//	_Inverter2.Phase3CurrentFrameTPDO = CanDataFrameBase(0x398,4);
//	_Inverter2.RotationSpeedTPDO = CanDataFrameBase(0x399,8);
//	_Inverter2.DCBusVoltage = CanDataFrameBase(0x400,4);
//	_Inverter2.ConsumedPowerInkW = CanDataFrameBase(0x401,8);
//	/* Inverter2 INIT END*/
//
//	/* MPPT1 INIT BEGIN*/
//	_MPPT1.ReceivedCurrentAndVoltage1SDO = CanDataFrameBaseWithAdditionalData(0x5B3, 8, 0x01);
//	MakeDataFrame(&_MPPT1, SDO_Receive_Byte0, 0xAA,  0xAA, 0x01, 0, 0, 0, 0);
//
//	_MPPT1.ReceivedCurrentAndVoltage2SDO = CanDataFrameBaseWithAdditionalData(0x5B3, 8, 0x02);
//	MakeDataFrame(&_MPPT1, SDO_Receive_Byte0, 0xAA,  0xAA, 0x02, 0, 0, 0, 0);
//
//	_MPPT1.ReceivedOperationalStateSDO = CanDataFrameBaseWithAdditionalData(0x5B3, 5, 0x03);
//	MakeDataFrame(&_MPPT1, SDO_Receive_Byte0, 0xAA,  0xAA, 0x03, 0, 0, 0, 0);
//
//	_MPPT1.SendCurrentAndVoltage1SDO = CanDataFrameBaseWithAdditionalData(0x633, 4, 0x01);
//	MakeDataFrame(&_MPPT1, SDO_Send_Byte0, 0xAA,  0xAA, 0x01, 0, 0, 0, 0);
//
//	_MPPT1.SendCurrentAndVoltage2SDO = CanDataFrameBaseWithAdditionalData(0x633, 4, 0x02);
//	MakeDataFrame(&_MPPT1, SDO_Send_Byte0, 0xAA,  0xAA, 0x02, 0, 0, 0, 0);
//
//	_MPPT1.SendOperationalStateSDO = CanDataFrameBaseWithAdditionalData(0x633, 4, 0x03);
//	MakeDataFrame(&_MPPT1, SDO_Send_Byte0, 0xAA,  0xAA, 0x03, 0, 0, 0, 0);
//	/* MPPT1 INIT END*/
//
//	/* MPPT2 INIT BEGIN*/
//	_MPPT2.ReceivedCurrentAndVoltage1SDO = CanDataFrameBaseWithAdditionalData(0x5B6, 8, 0x01);
//	MakeDataFrame(&_MPPT2, SDO_Receive_Byte0, 0xAA,  0xAA, 0x01, 0, 0, 0, 0);
//
//	_MPPT2.ReceivedCurrentAndVoltage2SDO = CanDataFrameBaseWithAdditionalData(0x5B6, 8, 0x02);
//	MakeDataFrame(&_MPPT2, SDO_Receive_Byte0, 0xAA,  0xAA, 0x02, 0, 0, 0, 0);
//
//	_MPPT2.ReceivedOperationalStateSDO = CanDataFrameBaseWithAdditionalData(0x5B6, 5, 0x03);
//	MakeDataFrame(&_MPPT2, SDO_Receive_Byte0, 0xAA,  0xAA, 0x03, 0, 0, 0, 0);
//
//	_MPPT2.SendCurrentAndVoltage1SDO = CanDataFrameBaseWithAdditionalData(0x636, 4, 0x01);
//	MakeDataFrame(&_MPPT2, SDO_Send_Byte0, 0xAA,  0xAA, 0x01, 0, 0, 0, 0);
//
//	_MPPT2.SendCurrentAndVoltage2SDO = CanDataFrameBaseWithAdditionalData(0x636, 4, 0x02);
//	MakeDataFrame(&_MPPT2, SDO_Send_Byte0, 0xAA,  0xAA, 0x02, 0, 0, 0, 0);
//
//	_MPPT2.SendOperationalStateSDO = CanDataFrameBaseWithAdditionalData(0x636, 4, 0x03);
//	MakeDataFrame(&_MPPT2, SDO_Send_Byte0, 0xAA,  0xAA, 0x03, 0, 0, 0, 0);
//	/* MPPT2 INIT END*/
//
//	/* MPPT3 INIT BEGIN*/
//	_MPPT3.ReceivedCurrentAndVoltage1SDO = CanDataFrameBaseWithAdditionalData(0x5B9, 8, 0x01);
//	MakeDataFrame(&_MPPT3, SDO_Receive_Byte0, 0xAA,  0xAA, 0x01, 0, 0, 0, 0);
//
//	_MPPT3.ReceivedCurrentAndVoltage2SDO = CanDataFrameBaseWithAdditionalData(0x5B9, 8, 0x02);
//	MakeDataFrame(&_MPPT3, SDO_Receive_Byte0, 0xAA,  0xAA, 0x02, 0, 0, 0, 0);
//
//	_MPPT3.ReceivedOperationalStateSDO = CanDataFrameBaseWithAdditionalData(0x5B9, 5, 0x03);
//	MakeDataFrame(&_MPPT3, SDO_Receive_Byte0, 0xAA,  0xAA, 0x03, 0, 0, 0, 0);
//
//	_MPPT3.SendCurrentAndVoltage1SDO = CanDataFrameBaseWithAdditionalData(0x639, 4, 0x01);
//	MakeDataFrame(&_MPPT3, SDO_Send_Byte0, 0xAA,  0xAA, 0x01, 0, 0, 0, 0);
//
//	_MPPT3.SendCurrentAndVoltage2SDO = CanDataFrameBaseWithAdditionalData(0x639, 4, 0x02);
//	MakeDataFrame(&_MPPT3, SDO_Send_Byte0, 0xAA,  0xAA, 0x02, 0, 0, 0, 0);
//
//	_MPPT3.SendOperationalStateSDO = CanDataFrameBaseWithAdditionalData(0x639, 4, 0x03);
//	MakeDataFrame(&_MPPT3, SDO_Send_Byte0, 0xAA,  0xAA, 0x03, 0, 0, 0, 0);
//	/* MPPT3 INIT END*/
//
//	//_LightsController.LightsData = MakeCanDataFrameTxMessage(0x581, SDO_Download, 0xFF, 0xFF, 0X1, byte4, byte5, byte6, byte7, DLC, StdId)
//}
//
//
