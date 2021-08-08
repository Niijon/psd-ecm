/**
  ******************************************************************************
  * File Name          : canopen_object_dict.c
  * Description        : Zbior danych obiektow znajdujacych sie w sieci wg
  * 					 standardu CANopen
  ******************************************************************************
  */

#include <canopen_object_dict.h>

//While making SDO frame 0 byte must be SDO_Receive_Byte0, when receiving data and SDO_Send_Byte0 when sending data
CanDataFrameRxMessage MakeCanDataFrameRxMessage
		(UNSIGNED16 id, UNSIGNED8 byte0, UNSIGNED8 byte1,  UNSIGNED8 byte2,  UNSIGNED8 byte3, UNSIGNED8 byte4,
		UNSIGNED8 byte5,  UNSIGNED8 byte6,  UNSIGNED8 byte7, UNSIGNED32 DLC,UNSIGNED32 StdId,
		UNSIGNED32 FilterIndex){
	CanDataFrameRxMessage CDFR;
	CDFR.ID = id;
	CDFR.data[0] = byte0;
	CDFR.data[1] = byte1;
	CDFR.data[2] = byte2;
	CDFR.data[3] = byte3;
	CDFR.data[4] = byte4;
	CDFR.data[5] = byte5;
	CDFR.data[6] = byte6;
	CDFR.data[7] = byte7;
	CDFR.rx_header.DLC = DLC;
	CDFR.rx_header.IDE = 0x00000000U;
	CDFR.rx_header.RTR = 0x00000000U;
	CDFR.rx_header.StdId = StdId;
	CDFR.rx_header.FilterMatchIndex = FilterIndex;
	CDFR.rx_header.Timestamp = DISABLE;
	return CDFR;
}

CanDataFrameTxMessage MakeCanDataFrameTxMessage
						(UNSIGNED16 id,UNSIGNED8 byte0, UNSIGNED8 byte1,  UNSIGNED8 byte2,  UNSIGNED8 byte3,
						UNSIGNED8 byte4,  UNSIGNED8 byte5,  UNSIGNED8 byte6,  UNSIGNED8 byte7,
						UNSIGNED32 DLC, UNSIGNED32 StdId){
	CanDataFrameTxMessage CDFT;
	CDFT.ID = id;
	CDFT.data[0] = byte0;
	CDFT.data[1] = byte1;
	CDFT.data[2] = byte2;
	CDFT.data[3] = byte3;
	CDFT.data[4] = byte4;
	CDFT.data[5] = byte5;
	CDFT.data[6] = byte6;
	CDFT.data[7] = byte7;
	CDFT.tx_header.DLC = DLC;
	CDFT.tx_header.IDE = 0x00000000U;
	CDFT.tx_header.RTR = 0x00000000U;
	CDFT.tx_header.StdId = StdId;
	CDFT.tx_header.TransmitGlobalTime = DISABLE;
	return CDFT;
}

void CanopenObjectDictInit() {
//	dashboard.node_id = 0x70;
//	dashboard.sdo_download_id = 0x581;		// Channel with ecm
//	dashboard.emcy_id = 0xF0;
//
//	inverter_1.node_id = 0x10;
//	inverter_1.pdo_producer_id = 0x290;	// 0x290 - 0x296 Channel with ecm
//	inverter_1.emcy_id = 0x90;
//
//	inverter_2.node_id = 0x15;
//	inverter_2.pdo_producer_id = 0x395;	// 0x395 - 0x401 Channel with ecm
//	inverter_2.emcy_id = 0x95;
//
//	mppt_1.node_id = 0x33;
//	mppt_1.sdo_upload_id = 0x633;		// Channel with ecm
//	mppt_1.sdo_download_id = 0x5B3;
//	mppt_1.emcy_id = 0xB3;
//	mppt_1.index = 0xAAAB;
//	mppt_1.subindex_1 = 0x01;
//	mppt_1.subindex_2 = 0x02;
//	mppt_1.subindex_3 = 0x03;			// 0x01 - 0x03
//
//	mppt_2.node_id = 0x36;
//	mppt_2.sdo_upload_id = 0x636;		// Channel with ecm
//	mppt_2.sdo_download_id = 0x5B6;
//	mppt_2.emcy_id = 0xB6;
//	mppt_2.index = 0xAAAA;
//	mppt_2.subindex_1 = 0x01;
//	mppt_2.subindex_2 = 0x02;
//	mppt_2.subindex_3 = 0x03;			// 0x01 - 0x03
//
//	mppt_3.node_id = 0x39;
//	mppt_3.sdo_upload_id = 0x639;		// Channel with ecm
//	mppt_3.sdo_download_id = 0x5B9;
//	mppt_3.emcy_id = 0xB9;
//	mppt_3.index = 0xAAAA;
//	mppt_3.subindex_1 = 0x01;
//	mppt_3.subindex_2 = 0x02;
//	mppt_3.subindex_3 = 0x03; 			// 0x01 - 0x03
//
//	ecm.node_id = 0x01;
//	ecm.sdo_upload_id = 0x581;
//	ecm.emcy_id = 0x81;
//
//	bms.node_id = 0x05;
//	bms.pdo_producer_id = 0x185;			// 0x185 - 0x196
//	bms.emcy_id = 0x85;
//	bms.sdo_download_id = 0x15;
//	bms.sdo_upload_id = 0x69;
//
	lights_controller.node_id = 0x50;
//	lights_controller.pdo_consumer_id = 0x581;
}

void CanOpenObjectsInit()
{
	_Dashboard.node_id = 0x70;
	_Dashboard.BMSInformationFramePDO = MakeCanDataFrameTxMessage(0X190, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 7, 0X190);


	//_LightsController.LightsData = MakeCanDataFrameTxMessage(0x581, SDO_Download, 0xFF, 0xFF, 0X1, byte4, byte5, byte6, byte7, DLC, StdId)
}


