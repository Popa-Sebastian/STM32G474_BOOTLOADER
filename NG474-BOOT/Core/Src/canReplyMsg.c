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
