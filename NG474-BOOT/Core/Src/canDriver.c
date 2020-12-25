/**
  ******************************************************************************
  * @file           : canDriver.c
  * @brief          : Functions related to CAN API
  * ----------------------------------------------------------------------------
  * @author         : Sebastian Popa
  * @date           : Dec, 2020
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "canDriver.h"
#include "bootloader.h"
#include "canReplyMsg.h"

/* Variables declared elsewhere-----------------------------------------------*/
extern FDCAN_HandleTypeDef hfdcan1; // declared in fdcan.c

/* Private variables ---------------------------------------------------------*/
// CAN TypeDefs
FDCAN_TxHeaderTypeDef   TxHeader;
FDCAN_RxHeaderTypeDef   RxHeader;

// User variables
uint64_t Received_Data64[32];
uint32_t received_data_index = 0;

// Transmit Data:

#if FLASH_TEST_DATA > 0
/* Test Data to be programmed */
uint64_t Data64_to_write[32] = {
  0x0000000000000000, 0x1111111111111111, 0x2222222222222222, 0x3333333333333333,
  0x4444444444444444, 0x5555555555555555, 0x6666666666666666, 0x7777777777777777,
  0x8888888888888888, 0x9999999999999999, 0xAAAAAAAAAAAAAAAA, 0xBBBBBBBBBBBBBBBB,
  0xCCCCCCCCCCCCCCCC, 0xDDDDDDDDDDDDDDDD, 0xEEEEEEEEEEEEEEEE, 0xFFFFFFFFFFFFFFFF,
  0x0011001100110011, 0x2233223322332233, 0x4455445544554455, 0x6677667766776677,
  0x8899889988998899, 0xAABBAABBAABBAABB, 0xCCDDCCDDCCDDCCDD, 0xEEFFEEFFEEFFEEFF,
  0x2200220022002200, 0x3311331133113311, 0x6644664466446644, 0x7755775577557755,
  0xAA88AA88AA88AA88, 0xBB99BB99BB99BB99, 0xEECCEECCEECCEECC, 0xFFDDFFDDFFDDFFDD
};
#endif

// Received Data:
uint8_t RxData[8];

/* Functions declaration- ----------------------------------------------------*/
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
void can_init(void)
{
	// Step1: Configure the FDCAN filters
	can_filter_init();

	// Step2: Configure global TxHeader attributes
	TxHeader.IdType =           FDCAN_STANDARD_ID;
	TxHeader.TxFrameType =      FDCAN_DATA_FRAME;
	TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader.BitRateSwitch =    FDCAN_BRS_OFF;
	TxHeader.FDFormat =         FDCAN_CLASSIC_CAN;
	TxHeader.TxEventFifoControl = FDCAN_STORE_TX_EVENTS;
	TxHeader.MessageMarker =    0x00;

	// Step3: Activate Interrupt Notifications
	if (HAL_FDCAN_ActivateNotification(&hfdcan1,
		  (FDCAN_IT_RX_FIFO0_NEW_MESSAGE), 0) != HAL_OK)
	{
		Error_Handler();
	}

#if USE_TX_INTERRUPT > 0
	if (HAL_FDCAN_ActivateNotification(&hfdcan1,
		  (FDCAN_IT_TX_FIFO_EMPTY), 0) != HAL_OK)
	{
	    Error_Handler();
	}
#endif

	// Step4: Start FDCAN controller (continuous listening CAN bus)
	if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK)
	{
		Error_Handler();
	}

	// Step5: Send a "Hello !!" message
	TxHeader.Identifier = 0x00;
	TxHeader.DataLength = FDCAN_DLC_BYTES_8;
	uint8_t TxHello[8]  = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x21, 0x21};
	HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxHello);
}

/**
  * @brief	Initializes CAN filters:
  * 		1) Index 0, range 000 - 0FF, Host instructions
  * 		2) Index 1, range 100 - 1FF, Data frames
  * @param	None
  * @retval	None
  */
void can_filter_init(void)
{
	FDCAN_FilterTypeDef     sFilterConfig;

	// Configure global filter parameters
	sFilterConfig.IdType =       FDCAN_STANDARD_ID;       // STD ID
	sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0; // All msgs to FIFO0

	// 1) Configure FilterIndex 0 [000 - 0FF] to receive HOST commands
	sFilterConfig.FilterIndex = CAN_HOST;
	sFilterConfig.FilterType =  FDCAN_FILTER_RANGE;
	sFilterConfig.FilterID1 =   0x000;
	sFilterConfig.FilterID2 =   0x0FF;
	if (HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig) != HAL_OK)
	{
		Error_Handler();
	}

	// 2) Configure FilterIndex 1 [100 - 1FF] to receive data frames
	sFilterConfig.FilterIndex = CAN_DATA;
	sFilterConfig.FilterType =  FDCAN_FILTER_RANGE;
	sFilterConfig.FilterID1 =   0x100;
	sFilterConfig.FilterID2 =   0x1FF;
	if (HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
  * @brief	Converts an array of 8 uint8_t elements to a uint64_t
  * @param	Array is an array of 8 uint8_t elements
  * @retval	uint64_t converted value
  */
uint64_t array_to_uint64(uint8_t *Array)
{
	int array_index;
	uint64_t converted_value = 0;
	for (array_index = 0; array_index < 7; array_index++)
	{
	  converted_value += Array[array_index];
	  converted_value = (converted_value << 8);
	}
	converted_value += Array[7];
	return converted_value;
}

#if USE_TX_INTERRUPT > 0
void HAL_FDCAN_TxFifoEmptyCallback(FDCAN_HandleTypeDef *hfdcan)
{
	TxHeader.Identifier = 0x200;
	HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxData);
}
#endif

/**
  * @brief	Message Received in Fifo0 Callback.
  * 		- This function is called by the HAL_FDCAN_IRQHandler. The IRQHandler
  * 		determines the cause of the interrupt, acknowledges the interrupt
  * 		flag and calls the specific function callback.
  * 		- This Interrupt Service Routine is called every time a new CAN message
  * 		is received in FIFO0.
  * 		- Message data is interpreted and stored in an array to be written in
  * 		flash memory.
  * 		- An echo of the data is resent to host to check data and index number
  * 		integrity.
  * 		- After 32 data messages have been received, data is stored in flash
  * 		memory at the specified user location. Start and end of flash write
  * 		are confirmed by CAN transmissions.
  * @param	FDCAN_HandleTypeDef
  * @param	RxFifo0ITs can determine the cause of interrupt if multiple interrupts
  * 		are enabled for FIFO0.
  * @retval	None
  */
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
	// Get message
	HAL_FDCAN_GetRxMessage(&hfdcan1, FDCAN_RX_FIFO0, &RxHeader, RxData);

	// Check what type of message it is (Host, Data)
	switch (RxHeader.FilterIndex)
		{
		case CAN_HOST:
			can_host_handler();
			break;

		case CAN_DATA:
			can_data_handler(RxHeader.Identifier, RxData);
			break;

		default:
			// Other modes are not supported
			break;
		}
}

/**
  * @brief	Handles HOST commands received in CAN Rx Callback
  *			TODO: implement this function
  * @param	None
  * @retval	None
  */
void can_host_handler(void)
{
	//do nothing
}

/**
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
void can_data_handler(uint32_t Identifier, uint8_t *rxdata_pt)
{
	// Check if this is the right Data - index
	if (Identifier != (0x100 + received_data_index))
	{
		// Error, data frames not in order
		uint8_t expected_index = (uint8_t)(received_data_index);
		uint8_t received_index = (uint8_t)(Identifier - 0x100);
		can_error_wrong_index(expected_index, received_index);
	} else
	{
		// Convert to uint_64
		Received_Data64[received_data_index] = array_to_uint64(rxdata_pt);

		// Send ACK - echo data frames;
		can_ack_echo_data(received_data_index, rxdata_pt);

		// Increment data index
		received_data_index++;

		// Check for Page complete
		if (received_data_index == 32u)
		{
			received_data_index = 0;

			// Send ACK - Page complete, 32 values received
			can_ack_page_complete();

			// Write page
			if (bootloader_FlashWrite(FLASH_USER_START_ADDR, Received_Data64) == HAL_OK)
			{
				// Send ACK - Write page complete
				can_ack_flash_complete();
			} else
			{
				// Send error
				can_error_flash();
			}
		}
	} // else
}

