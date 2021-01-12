/**
  ******************************************************************************
  * @file           : timer.c
  * @brief          : Source file containing function prototypes and defines
  *                   related to timers.
  * ----------------------------------------------------------------------------
  * @author         : Sebastian Popa
  * @date           : Jan, 2021
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "bootloader.h"
#include "uartMsg.h"

/* Variables declared elsewhere-----------------------------------------------*/
extern TIM_HandleTypeDef htim16; // declared in tim.c
extern uint32_t start_of_user_flash; // declared in canDriver.c
extern FDCAN_HandleTypeDef hfdcan1; // declared in fdcan.c

/* Functions declaration- ----------------------------------------------------*/
/***************************start_timer*****************************************
 **
  * @brief	Starts Timer 16 that waits for 5 sec to receive a message from host
  * 		to enter bootloader mode. If message is received, timer is stopped.
  * 		Else, in the timer interrupt HAL_TIM_PeriodElapsedCallback, jump to
  * 		user app is performed.
  * @param	None
  * @retval	None
  */
void start_timer(void)
{
	HAL_TIM_Base_Start_IT(&htim16);
}

/******************HAL_TIM_PeriodElapsedCallback********************************
 **
  * @brief	Timer16 interrupt handler. After time is elapsed, with no start
  * 		bootloader mode from host, jump to user app function is called.
  * @param	htim, determine which timer caused the interrupt (if more timers are
  * 		used).
  * @retval	None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	// toggle LED
	 HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	 uart_send_msg(".");

	 static uint32_t count = 0;
	 count++;
	 if (count == 20)
	 {
		 HAL_TIM_Base_Stop_IT(&htim16);	// stop timer16
		 HAL_FDCAN_Stop(&hfdcan1);		// stop CAN
		 uart_send_msg("\r\nAuto-jump to user app\r\n");
		 bootloader_JumpToUserApp(start_of_user_flash);
	 }
}

/***************************END OF FILE****************************************/
