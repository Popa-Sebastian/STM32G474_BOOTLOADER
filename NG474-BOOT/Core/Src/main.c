/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fdcan.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define FLASH_USER_START_ADDR   ADDR_FLASH_PAGE_128   /* FOR TESTING WE USE BANK2*/
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern FDCAN_HandleTypeDef hfdcan1;

/* Global Variable */
FDCAN_FilterTypeDef sFilterConfig;
FDCAN_TxHeaderTypeDef TxHeader;
FDCAN_RxHeaderTypeDef RxHeader;

uint8_t TxData[8] = {0x10, 0x32, 0x54, 0x76, 0x98, 0x00, 0x11, 0x22};
uint8_t page_complete_ack[1] = {0x00};
uint8_t write_complete_ack[1] = {0xFF};
uint8_t RxData[8];

/* Test Data to be programmed */
  uint64_t Data64_to_write[32] = {
  0x0000000000000000, 0x1111111111111111, 0x2222222222222222, 0x3333333333333333,
  0x4444444444444444, 0x5555555555555555, 0x6666666666666666, 0x7777777777777777,
  0x8888888888888888, 0x9999999999999999, 0xAAAAAAAAAAAAAAAA, 0xBBBBBBBBBBBBBBBB,
  0xCCCCCCCCCCCCCCCC, 0xDDDDDDDDDDDDDDDD, 0xEEEEEEEEEEEEEEEE, 0xFFFFFFFFFFFFFFFF,
  0x0011001100110011, 0x2233223322332233, 0x4455445544554455, 0x6677667766776677,
  0x8899889988998899, 0xAABBAABBAABBAABB, 0xCCDDCCDDCCDDCCDD, 0xEEFFEEFFEEFFEEFF,
  0x2200220022002200, 0x3311331133113311, 0x6644664466446644, 0x7755775577557755,
  0xAA88AA88AA88AA88, 0xBB99BB99BB99BB99, 0xEECCEECCEECCEECC, 0xFFDDFFDDFFDDFFDD};

  uint64_t Received_Data64[32];
  uint32_t received_data_index = 0;

  uint32_t flash_write_address;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void bootloader_JumpToUserApp(void);
void test_can_init(void);
void bootloader_FlashEraseBank2(void);
void bootloader_FlashWrite(uint32_t StartAddress, uint64_t *DATA_64);
uint64_t array_to_uint64(uint8_t *Array);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FDCAN1_Init();
  /* USER CODE BEGIN 2 */
  // bootloader_JumpToUserApp();
  test_can_init();
  // bootloader_FlashWrite(FLASH_USER_START_ADDR, Data64_to_write);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the peripherals clocks
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
  PeriphClkInit.FdcanClockSelection = RCC_FDCANCLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void bootloader_JumpToUserApp(void)
{
	// start of user flash
	uint32_t USER_FLASH_ADDRESS = 0x08008000U;

	// define a function pointer to user reset handler
	void (*user_reset_handler)(void);

	// set the MSP
	// MSP located at start of user flash (@0x0800 8000)
	uint32_t user_msp_value = *((volatile uint32_t *) USER_FLASH_ADDRESS);
	__set_MSP(user_msp_value);

	// reset handler address is the next location (@ 0x0800 8004)
	uint32_t user_reset_handler_address = *((volatile uint32_t*) (USER_FLASH_ADDRESS + 4U));
	user_reset_handler = (void*) user_reset_handler_address; // cast to function pointer

	// call of user_reset handler starts execution of user app
	user_reset_handler();
}

void test_can_init(void)
{
	// Configure the FDCAN filters
	sFilterConfig.IdType = FDCAN_STANDARD_ID;
	sFilterConfig.FilterIndex = 0;
	sFilterConfig.FilterType = FDCAN_FILTER_RANGE;
	sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
	sFilterConfig.FilterID1 = 0x000;
	sFilterConfig.FilterID2 = 0x7FF;
	if (HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig) != HAL_OK)
	{
	Error_Handler();
	}

	// Configure TxHeader
	TxHeader.Identifier = 0x00;
	TxHeader.IdType = FDCAN_STANDARD_ID;
	TxHeader.TxFrameType = FDCAN_DATA_FRAME;
	TxHeader.DataLength = FDCAN_DLC_BYTES_8;
	TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
	TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
	TxHeader.TxEventFifoControl = FDCAN_STORE_TX_EVENTS;
	TxHeader.MessageMarker = 0x00;

	// Activate Interrupt Notifications
	if (HAL_FDCAN_ActivateNotification(&hfdcan1,
		  (FDCAN_IT_RX_FIFO0_NEW_MESSAGE), 0) != HAL_OK)
	{
		Error_Handler();
	}
	// if (HAL_FDCAN_ActivateNotification(&hfdcan1,
	//		  (FDCAN_IT_TX_FIFO_EMPTY), 0) != HAL_OK)
	//{
	//	Error_Handler();
	//}

	// Start FDCAN controller (continuous listening CAN bus)
	if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK)
	{
		Error_Handler();
	}

	// Send a Hello message
	HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxData);
	TxHeader.Identifier = 0x200;

}

void HAL_FDCAN_TxFifoEmptyCallback(FDCAN_HandleTypeDef *hfdcan)
{
	// TxHeader.Identifier = 0x200;
	// HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxData);
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
	// Get message
	HAL_FDCAN_GetRxMessage(&hfdcan1, FDCAN_RX_FIFO0, &RxHeader, RxData);

	// Convert to uint_64
	Received_Data64[received_data_index] = array_to_uint64(RxData);
	received_data_index++;

	// Send ACK - resend frames
	HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, RxData);
	TxHeader.Identifier += 1u;

	if (received_data_index == 32u){
		received_data_index = 0;
		TxHeader.Identifier = 0x300;
		TxHeader.DataLength = FDCAN_DLC_BYTES_1;

		// Send ACK - Page complete, 32 values received
		HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, page_complete_ack);

		// Write page
		bootloader_FlashWrite(FLASH_USER_START_ADDR, Received_Data64);

		// Send ACK - Write page complete
		TxHeader.Identifier = 0x400;
		HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, write_complete_ack);

		TxHeader.Identifier = 0x200;
		TxHeader.DataLength = FDCAN_DLC_BYTES_8;
	}

}
/**
  * @brief	This function erases Bank2 of the memory. (page 128 - page 255)
  * @param 	None
  * @retval	None
  */
void bootloader_FlashEraseBank2(void)
{
	FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t BankNumber = 2;
	// uint32_t PageNumber;
	uint32_t PageError;
	//uint32_t src_addr = (uint32_t)DATA_64;

	/* Fill EraseInit structure*/
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_MASSERASE;
	EraseInitStruct.Banks = BankNumber;

	HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);
}
/**
  * @brief	This function writes 2KB starting from a given address
  * @param	StartAddress is the starting address from where to start the flash write
  * @param	DATA_64 is a pointer to an array of data to be written of size (32 x 64bit)
  * @retval	None
  */
void bootloader_FlashWrite(uint32_t StartAddress, uint64_t *DATA_64)
{
	HAL_FLASH_Unlock();

	// Delete memory bank 2 in order to be able to write
	bootloader_FlashEraseBank2();

	/* HAL_FLASH_Program() requires the address of the data to be flashed in integer
	 * format (e.g. 0x2000008) and not pointer format. So we need to do the following
	 * type casting.
	 */
	uint32_t data_address_uint = (uint32_t)DATA_64; // stores the address in integer format
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_FAST, StartAddress, (uint64_t)data_address_uint);

	HAL_FLASH_Lock();
}
/**
  * @brief	Converts an array of 8 uint8_t elements to a uint64_t
  * @param	Array is an array of 8 uint8_t elements
  * @retval	uint64_t converted value
  */
uint64_t array_to_uint64(uint8_t *Array)
{
	  int array_index;
	  uint64_t Converted_Array = 0;
	  for (array_index = 0; array_index < 7; array_index++)
	  {
		  Converted_Array += Array[array_index];
		  Converted_Array = (Converted_Array << 8);
	  }
	  Converted_Array += Array[7];
	  return Converted_Array;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
