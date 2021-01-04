/**
  ******************************************************************************
  * @file           : bootloader.h
  * @brief          : Header file containing function prototypes and defines
  *                   related to bootloader application.
  * ----------------------------------------------------------------------------
  * @author         : Sebastian Popa
  * @date           : Dec, 2020
  ******************************************************************************
  */
#ifndef INC_BOOTLOADER_H_
#define INC_BOOTLOADER_H_

/*********************bootloader_JumpToUserApp**********************************
 **
  * @brief  Jumps to the user application located at a specified memory location.
  *         Gets the user app Main Stack Pointer (MSP) and the address of the
  *         user app reset_handler() function.
  *         Calls user app reset_handler() to start execution of user app.
  * @param  None
  *         TODO: add input paramter of type uint32_t for memory address.
  * @retval None
  */
void bootloader_JumpToUserApp(void);

/*********************bootloader_FlashEraseBank2********************************
 **
  * @brief	This function erases Bank2 of the memory. (page 128 - page 255)
  * @param 	None
  * @retval	HAL_FLASH_GetError
  */
uint32_t bootloader_FlashEraseBank2(void);

/*********************bootloader_FlashErasePage*********************************
 **
  * @brief	This function erases a number of pages starting from the specified
  * 		page number
  * @param 	Page, starting page to begin erase
  * @param  Numberpages, how many pages to be erased
  * @retval HAL_FLASH_GetError
  */
uint32_t bootloader_FlashErasePage(uint32_t Page, uint32_t NumberPages);

/*********************bootloader_FlashWrite*************************************
 **
  * @brief	This function writes 2KB of data starting from a given address
  * @param	StartAddress is the starting address from where to start the flash write
  * @param	DATA_64 is a pointer to an array of data to be written of size (32 x 64bit)
  * @retval	HAL_FLASH_GetError
  */
uint32_t bootloader_FlashWrite(uint32_t StartAddress, uint64_t *DATA_64);

#endif /* INC_BOOTLOADER_H_ */

/***************************END OF FILE****************************************/
