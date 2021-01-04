/**
  ******************************************************************************
  * @file           : timer.h
  * @brief          : Header file containing function prototypes and defines
  *                   related to timers.
  * ----------------------------------------------------------------------------
  * @author         : Sebastian Popa
  * @date           : Jan, 2021
  ******************************************************************************
  */
#ifndef INC_TIMER_H_
#define INC_TIMER_H_

/* Functions declaration- ----------------------------------------------------*/
/**
  * @brief	Starts Timer 16 that waits for 5 sec to receive a message from host
  * 		to enter bootloader mode. If message is received, timer is stopped.
  * 		Else, in the timer interrupt HAL_TIM_PeriodElapsedCallback, jump to
  * 		user app is performed.
  * @param	None
  * @retval	None
  */
void start_timer(void);

#endif /* INC_TIMER_H_ */

/***************************END OF FILE****************************************/
