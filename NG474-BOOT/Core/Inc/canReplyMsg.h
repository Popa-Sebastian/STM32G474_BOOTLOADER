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
/**
  * @brief	Sends ack page complete frame
  * 		ID = 0x300,
  * 		DATA[1]
  * 		[1]: 0x00
  * @param	None
  * @retval	None
  */
void can_ack_page_complete(void);

/**
  * @brief	Sends ack flash complete frame
  * 		ID = 0x400,
  * 		DATA[1]
  * 		[1]: 0xFF
  * @param	None
  * @retval	None
  */
void can_ack_flash_complete(void);

/**
  * @brief	Sends ack and an echo of the received data
  * 		ID = 0x200 + data_index
  * 		DATA[8] = RxData
  * @param	data_index
  * @param	rxdata_pt points to data to be echoed back
  * @retval	None
  */
void can_ack_echo_data(uint32_t data_index, uint8_t *rxdata_pt);

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
void can_error_wrong_index(uint8_t expected_index, uint8_t received_index);

/**
  * @brief	Sends error frame when flash write fails
  * 		ID = 0x7FF
  * 		DATA[1] = 0XFF
  * @param	None
  * @retval	None
  */
void can_error_flash (void);

#endif /* INC_CANREPLYMSG_H_ */
