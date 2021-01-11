/**
  ******************************************************************************
  * @file           : bootloader.c
  * @brief          : Functions related to bootloader
  * ----------------------------------------------------------------------------
  * @author         : Sebastian Popa
  * @date           : Dec, 2020
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "bootloader.h"
#include "uartMsg.h"

/* Private variables ---------------------------------------------------------*/
FLASH_EraseInitTypeDef EraseInitStruct;

/* Functions declaration- ----------------------------------------------------*/
/*********************bootloader_JumpToUserApp**********************************
 **
  * @brief  Jumps to the user application located at a specified memory location.
  *         Gets the user app Main Stack Pointer (MSP) and the address of the
  *         user app reset_handler() function.
  *         Calls user app reset_handler() to start execution of user app.
  * @param  user_flash represents start of user flash memory address
  * @retval None
  */
void bootloader_JumpToUserApp(uint32_t user_flash)
{
	uart_send_msg("\r\nJumping to user app\r\n");
	// define a function pointer to user reset handler
	void (*user_reset_handler)(void);

	// set the MSP
	// MSP located at start of user flash (eg: @0x0800 8000)
	uint32_t user_msp_value = *((volatile uint32_t *) user_flash);
	__set_MSP(user_msp_value);

	// reset handler address is the next location (eg: @ 0x0800 8004)
	uint32_t user_reset_handler_address = *((volatile uint32_t*) (user_flash + 4U));
	user_reset_handler = (void*) user_reset_handler_address; // cast to function pointer

	// call of user_reset handler starts execution of user app
	user_reset_handler();
}

/*********************bootloader_FlashEraseBank2********************************
 **
  * @brief	This function erases Bank2 of the memory. (page 128 - page 255)
  * @param 	None
  * @retval	HAL_FLASH_GetError
  */
uint32_t bootloader_FlashEraseBank2(void)
{	uint32_t Status = HAL_OK;
	HAL_FLASH_Unlock();
	uint32_t BankNumber = 2;
	// uint32_t PageNumber;
	uint32_t PageError;
	//uint32_t src_addr = (uint32_t)DATA_64;

	/* Fill EraseInit structure*/
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_MASSERASE;
	EraseInitStruct.Banks = BankNumber;

	if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
	{
		Status = HAL_FLASH_GetError();
	}
	HAL_FLASH_Lock();
	return Status;
}

/*********************bootloader_FlashErasePage*********************************
 **
  * @brief	This function erases a number of pages starting from the specified
  * 		page number
  * @param 	Page, starting page to begin erase
  * @retval HAL_FLASH_GetError
  */
uint32_t bootloader_FlashErasePage(uint32_t Page)
{
	uint32_t Status = HAL_OK;

	/* Unlock the Flash to enable the flash control register access *************/
	HAL_FLASH_Unlock();

	/* Clear OPTVERR bit set on virgin samples */
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

	uint32_t PageError;

	uint32_t BankNumber;
	uint32_t NumberPages;

	if (Page < 128) {
		BankNumber = FLASH_BANK_1;
		NumberPages = 127 - Page + 1;
	} else {
		BankNumber = FLASH_BANK_2;
		NumberPages = 255 - Page + 1;
	}

	/* Fill EraseInit structure*/
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.Banks     =	BankNumber;
	EraseInitStruct.Page      =	Page;
	EraseInitStruct.NbPages   =	NumberPages;

	if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
	{
		Status = HAL_FLASH_GetError();
	}
	HAL_FLASH_Lock();
	return Status;
}
/*********************bootloader_FlashWrite*************************************
 **
  * @brief	This function writes 2KB of data starting from a given address
  * @param	StartAddress is the starting address from where to start the flash write
  * @param	DATA_64 is a pointer to an array of data to be written of size (32 x 64bit)
  * @retval	HAL_FLASH_GetError
  */
uint32_t bootloader_FlashWrite(uint32_t StartAddress, uint64_t *DATA_64)
{
	uint32_t Status = HAL_OK;
	HAL_FLASH_Unlock();
	/* HAL_FLASH_Program() requires the address of the data to be flashed in u_integer
	 * format (e.g. 0x2000008) and not pointer format. So we need to do the following
	 * type casting.
	 */
	uint32_t data_address_uint = (uint32_t)DATA_64; // stores the address in integer format
	if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_FAST, StartAddress, (uint64_t)data_address_uint) != HAL_OK)
	{
		Status = HAL_FLASH_GetError();
	}
	HAL_FLASH_Lock();
	return Status;
}

/***************************END OF FILE****************************************/
