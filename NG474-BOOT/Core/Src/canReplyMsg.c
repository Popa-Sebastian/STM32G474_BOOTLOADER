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

/* Variables declared elsewhere-----------------------------------------------*/
extern FDCAN_HandleTypeDef 	 hfdcan1;  // declared in fdcan.c
extern FDCAN_TxHeaderTypeDef TxHeader; // declared in canDriver.c

/* Private variables ---------------------------------------------------------*/

/* Functions declaration- ----------------------------------------------------*/
/**
  * @brief	Sends ack page complete frame
  * 		ID = 0x300,
  * 		DATA[1]
  * 		[1]: 0x00
  * @param	None
  * @retval	None
  */
void can_ack_page_complete(void)
{
	uint8_t page_complete_ack[1]  = {0x00};
	TxHeader.Identifier = 0x300;
	TxHeader.DataLength = FDCAN_DLC_BYTES_1;
	HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, page_complete_ack);
}

/**
  * @brief	Sends ack flash complete frame
  * 		ID = 0x400,
  * 		DATA[1]
  * 		[1]: 0xFF
  * @param	None
  * @retval	None
  */
void can_ack_flash_complete(void)
{
	uint8_t write_complete_ack[1] = {0xFF};
	TxHeader.Identifier = 0x400;
	TxHeader.DataLength = FDCAN_DLC_BYTES_1;
	HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, write_complete_ack);
}

/**
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

/**
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