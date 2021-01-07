/**
  ******************************************************************************
  * @file           : canReplyMsg.c
  * @brief          : Reply messages send to host to indicate status
  * ----------------------------------------------------------------------------
  * @author         : Sebastian Popa
  * @date           : Dec, 2020
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "canReplyMsg.h"
#include "dataConversion.h"

/* Variables declared elsewhere-----------------------------------------------*/
extern FDCAN_HandleTypeDef 	 hfdcan1;  // declared in fdcan.c
extern FDCAN_TxHeaderTypeDef TxHeader; // declared in canDriver.c

/* Private variables ---------------------------------------------------------*/

/* Functions declaration- ----------------------------------------------------*/
/*********************can_ack_page_complete*************************************
 **
  * @brief	Sends ack page complete frame
  * 		ID = 0x300,
  * 		DATA[3]
  * 		[1]: frame_number
  * 		[2]: first byte of CRC
  * 		[3]: second byte of CRC
  * @param	frame_number
  * @retval	None
  */
void can_ack_page_complete(uint32_t frame_number, uint32_t crc)
{
	uint8_t page_complete_ack[3];
	uint8_t crc_array[2];
	TxHeader.Identifier = 0x300;
	TxHeader.DataLength = FDCAN_DLC_BYTES_3;

	// Convert CRC to byte format
	uint16_to_array(crc, crc_array);

	page_complete_ack[0] = frame_number;
	page_complete_ack[1] = crc_array[0];
	page_complete_ack[2] = crc_array[1];
	HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, page_complete_ack);
}

/*********************can_ack_flash_complete************************************
 **
  * @brief	Sends ack flash complete frame
  * 		ID = 0x400,
  * 		DATA[1]
  * 		[1]: 0xFF
  * @param	frame number to be sent as ACK
  * @retval	None
  */
void can_ack_flash_complete(uint32_t frame_number)
{
	uint8_t write_complete_ack[1];
	write_complete_ack[0] = frame_number;
	TxHeader.Identifier = 0x400;
	TxHeader.DataLength = FDCAN_DLC_BYTES_1;
	HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, write_complete_ack);
}

/***********************can_ack_echo_data***************************************
 **
  * @brief	Sends ack and an echo of the received data
  * 		ID = 0x200 + data_index
  * 		DATA[8] = RxData
  * @param	data_index
  * @param	rxdata_pt points to data to be echoed back
  * @retval	None
  */
void can_ack_echo_data(uint32_t data_index, uint8_t *rxdata_pt)
{
	TxHeader.Identifier = 0x200 + data_index;
	TxHeader.DataLength = FDCAN_DLC_BYTES_8;
	HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, rxdata_pt);
}

/**********************can_error_wrong_index************************************
 **
  * @brief	Sends a error frame if the received index is different than expected
  * 		ID = 0x2FF,
  * 		DATA[3]
  * 		[1]: 0xFF
  * 		[2]: expected index
  * 		[3]: received index
  * @param	expected_index
  * @param	received_index
  * @retval	None
  */
void can_error_wrong_index(uint8_t expected_index, uint8_t received_index)
{
	TxHeader.Identifier = 0x2FF;
	TxHeader.DataLength = FDCAN_DLC_BYTES_3;
	uint8_t error_wrong_index[3];
	error_wrong_index[0] = 0xFF;              // error notification
	error_wrong_index[1] = expected_index;    // expected index
	error_wrong_index[2] = received_index;    // received index
	HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, error_wrong_index);
}

/*************************can_error_flash***************************************
 **
  * @brief	Sends error frame when flash write fails
  * 		ID = 0x7FF
  * 		DATA[1] = 0XFF
  * @param	None
  * @retval	None
  */
void can_error_flash (void)
{
	TxHeader.Identifier = 0x7FF;
	TxHeader.DataLength = FDCAN_DLC_BYTES_1;
	uint8_t error_flash[1] = {0xFF};
	HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, error_flash);
}

/*************************can_ack_frame_reset***********************************
 **
  * @brief	Sends ack that current frame has been reset
  * 		ID = 0x20
  * 		DATA[1] = frame_number
  * @param	frame number
  * @retval	None
  */
void can_ack_frame_reset(uint32_t frame_number)
{
	// implement this function
}

/***************************END OF FILE****************************************/
