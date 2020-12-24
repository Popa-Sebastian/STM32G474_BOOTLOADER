/**
  ******************************************************************************
  * @file           : canDriver.h
  * @brief          : Header file containing function prototypes and defines
  *                   related to CAN communication with HOST application.
  * ----------------------------------------------------------------------------
  * @author         : Sebastian Popa
  * @date           : Dec, 2020
  ******************************************************************************
  */
#ifndef INC_CANDRIVER_H_
#define INC_CANDRIVER_H_

/* Private define ------------------------------------------------------------*/
#define USE_TX_INTERRUPT        0
#define FLASH_TEST_DATA         0
#define FLASH_USER_START_ADDR   ADDR_FLASH_PAGE_128   /* FOR TESTING WE USE BANK2*/

/* Private function prototypes -----------------------------------------------*/
/**
  * @brief	Initializes Can Driver API:
  * 		1) Configures receive filters
  * 		2) Configures global TxHeader
  * 		3) Activates Interrupts
  * 		4) Starts CAN controller
  * 		5) Sends a Hello message (CAN Init OK)
  * @param	None
  * @retval	None
  */
void can_init(void);

/**
  * @brief	Converts an array of 8 uint8_t elements to a uint64_t
  * @param	Array is an array of 8 uint8_t elements
  * @retval	uint64_t converted value
  */
uint64_t array_to_uint64(uint8_t *Array);

#endif /* INC_CANDRIVER_H_ */
