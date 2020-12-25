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

#endif /* INC_CANREPLYMSG_H_ */
