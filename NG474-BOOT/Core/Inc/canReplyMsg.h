/**
  ******************************************************************************
  * @file           : canReplyMsg.h
  * @brief          : Reply messages send to host to indicate status
  * ----------------------------------------------------------------------------
  * @author         : Sebastian Popa
  * @date           : Dec, 2020
  ******************************************************************************
  */
#ifndef INC_CANREPLYMSG_H_
#define INC_CANREPLYMSG_H_

/* Private function prototypes -----------------------------------------------*/
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
void can_ack_page_complete(uint32_t frame_number, uint32_t crc);

/*********************can_ack_flash_complete************************************
 **
  * @brief	Sends ack flash complete frame
  * 		ID = 0x400,
  * 		DATA[1]
  * 		[1]: 0xFF
  * @param	frame number to be sent as ACK
  * @retval	None
  */
void can_ack_flash_complete(uint32_t frame_number);

/***********************can_ack_echo_data***************************************
 **
  * @brief	Sends ack and an echo of the received data
  * 		ID = 0x200 + data_index
  * 		DATA[8] = RxData
  * @param	data_index
  * @param	rxdata_pt points to data to be echoed back
  * @retval	None
  */
void can_ack_echo_data(uint32_t data_index, uint8_t *rxdata_pt);

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
void can_error_wrong_index(uint8_t expected_index, uint8_t received_index);

/*************************can_error_flash***************************************
 **
  * @brief	Sends error frame when flash write fails
  * 		ID = 0x7FF
  * 		DATA[1] = 0XFF
  * @param	None
  * @retval	None
  */
void can_error_flash (void);

/*************************can_ack_frame_reset***********************************
 **
  * @brief	Sends ack that current frame has been reset
  * 		ID = 0x20
  * 		DATA[1] = frame_number
  * @param	frame number
  * @retval	None
  */
void can_ack_frame_reset(uint32_t frame_number);

#endif /* INC_CANREPLYMSG_H_ */

/***************************END OF FILE****************************************/
