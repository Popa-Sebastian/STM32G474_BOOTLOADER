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
