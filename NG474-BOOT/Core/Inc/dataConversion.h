/**
  ******************************************************************************
  * @file           : dataConversion.h
  * @brief          : Functions that convert from array elements do single data
  * 				  element
  * ----------------------------------------------------------------------------
  * @author         : Sebastian Popa
  * @date           : Dec, 2020
  ******************************************************************************
  */

#ifndef INC_DATACONVERSION_H_
#define INC_DATACONVERSION_H_

/* Private function prototypes -----------------------------------------------*/
/**
  * @brief	Converts an array of 8 uint8_t elements to a uint64_t
  * @param	Array is an array of 8 uint8_t elements
  * @retval	uint64_t converted value
  */
uint64_t array_to_uint64(uint8_t *Array);

/**
  * @brief	Converts an array of 4 uint8_t elements to a uint32_t
  * @param	Array is an array of 4 uint8_t elements
  * @retval	uint32_t converted value
  */
uint64_t array_to_uint32(uint8_t *Array);

#endif /* INC_DATACONVERSION_H_ */

/***************************END OF FILE****************************************/
