/**
  ******************************************************************************
  * @file           : uartMsg.c
  * @brief          : Functions to send messages through USART1
  * ----------------------------------------------------------------------------
  * @author         : Sebastian Popa
  * @date           : Jan, 2021
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "uartMsg.h"

/* Variables declared elsewhere-----------------------------------------------*/
extern UART_HandleTypeDef huart1; // defined in usart.c

/* Private function prototypes -----------------------------------------------*/
/**************************uart_send_msg****************************************
 **
  * @brief	Sends a message through UART
  * @param	user_data
  * @retval	status
  */
HAL_StatusTypeDef uart_send_msg(char *user_data)
{
	HAL_StatusTypeDef Status;
	Status = HAL_UART_Transmit(&huart1, (uint8_t*)user_data,
			strlen(user_data), UART_DELAY);

	return Status;
}

/***************************END OF FILE****************************************/
