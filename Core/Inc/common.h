/*******************************************************************************
 * Filename:			common.h
 * Revised:				Date: 2019.11.04
 * Revision:			V001
 * Description:		    All common define, enumrate, structure
*******************************************************************************/

#ifndef _COMMON_H_
#define _COMMON_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * INCLUDES
 ******************************************************************************/
// Standard C Lbrary Header
#include "stdarg.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"

// ST Library Header
#include "stm32f4xx_hal.h"

uint32_t _write(uint32_t file, char *ptr, uint32_t len)
{
	uint32_t i = 0;
	for (i = 0; i < len; i++)
	{
		ITM_SendChar((*ptr++));
	}
	return len;
}

/*******************************************************************************
 * CONSTANTS
 ******************************************************************************/

/*******************************************************************************
 * ENUMERATED
 ******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _COMMON_H_ */
