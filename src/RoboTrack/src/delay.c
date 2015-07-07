/*
 * delay.c
 *
 *  Created on: 11.11.2012
 *      Author: Александр Горбачев
 */

#include "delay.h"

/**
 * @brief  Inserts a delay time.
 * @param  nCount: specifies the delay time length.
 * @retval None
 */
void simple_delay(__IO uint32_t nCount)
{
	/* Decrement nCount value */
	while (nCount != 0)
	{
		nCount--;
	}
}

/**
 * @brief  Inserts a delay time in miliseconds.
 * @param  nCount: specifies the delay time length.
 * @retval None
 */
void simple_delay_ms(__IO uint32_t nCount)
{
	/* Decrement nCount value */
	while (nCount != 0)
	{
		nCount--;
		simple_delay(SIMPLE_DELAY_CYCLES);
	}
}

/**
 * @brief  Inserts a delay time in miliseconds.
 * @param  nCount: specifies the delay time length.
 * @retval None
 */
void simple_delay_us(__IO uint32_t nCount)
{
	/* Decrement nCount value */
	while (nCount != 0)
	{
		nCount--;
		simple_delay(SIMPLE_DELAY_CYCLES / 1000);
	}
}
