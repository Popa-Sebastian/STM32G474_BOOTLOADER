/**
  ******************************************************************************
  * @file           : uartMsg.h
  * @brief          : Headers to unctions to send messages through USART1
  * ----------------------------------------------------------------------------
  * @author         : Sebastian Popa
  * @date           : Jan, 2021
  ******************************************************************************
  */

#ifndef INC_UARTMSG_H_
#define INC_UARTMSG_H_

/* Private defines------------ -----------------------------------------------*/
#define UART_DELAY	100    //Delay in ms
#define USE_UART	1

/* Private function prototypes -----------------------------------------------*/
/**************************uart_send_msg****************************************
 **
  * @brief	Sends a message through UART
  * @param	user_data
  * @retval	status
  */
HAL_StatusTypeDef uart_send_msg(char * user_data);

#endif /* INC_UARTMSG_H_ */

/***************************END OF FILE****************************************/
