/**
  ******************************************************************************
  * @file           : dataConversion.c
  * @brief          : Functions that convert from array elements do single data
  * 				  element
  * ----------------------------------------------------------------------------
  * @author         : Sebastian Popa
  * @date           : Dec, 2020
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Functions declaration- ----------------------------------------------------*/
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

/* Functions declaration- ----------------------------------------------------*/
/**
  * @brief	Converts an array of 4 uint8_t elements to a uint32_t
  * @param	Array is an array of 4 uint8_t elements
  * @retval	uint32_t converted value
  */
uint64_t array_to_uint32(uint8_t *Array)
{
	int array_index;
	uint32_t converted_value = 0;
	for (array_index = 0; array_index < 3; array_index++)
	{
	  converted_value += Array[array_index];
	  converted_value = (converted_value << 8);
	}
	converted_value += Array[3];
	return converted_value;
}
/***************************END OF FILE****************************************/

