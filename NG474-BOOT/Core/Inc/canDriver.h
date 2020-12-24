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

/* Private enums ------------------------------------------------------------*/
typedef enum
{
	CAN_HOST = 0, /*!< \brief host message */
	CAN_DATA = 1  /*!< \brief data message */
} message_type;

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
  * @brief	Initializes CAN filters:
  * 		1) Index 0, range 000 - 0FF, Host instructions
  * 		2) Index 1, range 100 - 1FF, Data frames
  * @param	None
  * @retval	None
  */
void can_filter_init(void);

/**
  * @brief	Handles data received in Can Rx Callback
  * 		1) Checks if data received is the right data frame expected
  * 		2) Message data is interpreted and stored in an array to be written in
  * 		flash memory.
  * 		3) An echo of the data is resent to host to check data and index number
  * 		integrity.
  * 		4) After 32 data messages have been received, data is stored in flash
  * 		memory at the specified user location. Start and end of flash write
  * 		are confirmed by CAN transmissions.
  * @param	None
  * @retval	None
  */
void can_data_handler(void);

/**
  * @brief	Handles HOST commands received in CAN Rx Callback
  * @param	None
  * @retval	None
  */
void can_host_handler(void);
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
void can_acK_flash_complete(void);

/**
  * @brief	Sends ack and an echo of the received data
  * 		ID = 0x200 + data_index
  * 		DATA[8] = RxData
  * @param	None
  * @retval	None
  */
void can_ack_echo_data(void);

/**
  * @brief	Sends a error frame if the received index is different than expected
  * 		ID = 0x2FF,
  * 		DATA[3]
  * 		[1]: 0xFF
  * 		[2]: expected index
  * 		[3]: received index
  * @param	None
  * @retval	None
  */
void can_error_wrong_index(void);

/**
  * @brief	Sends error frame when flash write fails
  * 		ID = 0x7FF
  * 		DATA[1] = 0XFF
  * @param	None
  * @retval	None
  */
void can_error_flash (void);

/**
  * @brief	Converts an array of 8 uint8_t elements to a uint64_t
  * @param	Array is an array of 8 uint8_t elements
  * @retval	uint64_t converted value
  */
uint64_t array_to_uint64(uint8_t *Array);

#endif /* INC_CANDRIVER_H_ */
