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
#define USE_CAN_RECEIVE_ACK		1
#define FLASH_USER_START_ADDR   ADDR_FLASH_PAGE_128   /* FOR TESTING WE USE BANK2*/

/* Private enums ------------------------------------------------------------*/
/**
  * @brief	Receive filters
  */
typedef enum
{
	CAN_HOST = 0, /*!< \brief host message */
	CAN_DATA = 1  /*!< \brief data message */
} message_type;

/**
  * @brief	HOST to bootloader commands
  */
typedef enum
{
	HOST_ENTER_BOOTLOADER = 0x00,
	HOST_USER_ADDRESS =     0x10,
	HOST_RESET_FRAME =      0x20,
	HOST_JUMP_TO_APP =      0x90
} host_command_type;

/* Private function prototypes -----------------------------------------------*/
/****************************can_init*******************************************
 **
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

/************************can_filter_init****************************************
 **
  * @brief	Initializes CAN filters:
  * 		1) Index 0, range 000 - 0FF, Host instructions
  * 		2) Index 1, range 100 - 1FF, Data frames
  * @param	None
  * @retval	None
  */
void can_filter_init(void);

/***************************can_data_handler************************************
 **
  * @brief	Handles data received in CAN Rx Callback
  * 		1) Checks if data received is the right data frame expected
  * 		2) Message data is interpreted and stored in an array to be written in
  * 		flash memory.
  * 		3) An echo of the data is resent to host to check data and index number
  * 		integrity.
  * 		4) After 32 data messages have been received, data is stored in flash
  * 		memory at the specified user location. Start and end of flash write
  * 		are confirmed by CAN transmissions.
  * @param	Identifier of the received message (corresponds to Data index)
  * @param  rxdata_pt - pointer to received data to be stored
  * @retval	None
  */
void can_data_handler(uint32_t Identifier, uint8_t *rxdata_pt);

/***************************can_host_handler************************************
 **
  * @brief	Handles HOST commands received in CAN Rx Callback
  *			Supported modes:
  *			1) HOST_ENTER_BOOTLOADER: after MCU reset, bootloader app waits for
  *			a determined amount of time and looks for a HOST_ENTER_BOOTLOADER
  *			command to enter bootloader mode.
  *			2) HOST_USER_ADDRESS: specifies the start of the user flash memory
  *			address, where the user app will be stored.
  *			3) HOST_JUMP_TO_APP: after a successful bootloader and flash of the
  *			of the user app, this command makes the jump to user app.
  * @param	Identifier corresponds to the command issued by HOST
  * @param  rxdata_pt, pointer to data
  * @retval	None
  */
void can_host_handler(uint32_t Identifier, uint8_t *rxdata_pt);

#endif /* INC_CANDRIVER_H_ */

/***************************END OF FILE****************************************/
