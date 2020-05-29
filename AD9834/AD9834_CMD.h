/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : AD9834_CMD.h
* Author             : Tiko Zhong
* Date First Issued  : Jan/24/2017
* Description        : This file provides a set of functions needed to manage the
*                      communication using HAL_UARTxxx
********************************************************************************
* History:
* 24/01/2017: V0.0
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _AD9834_CMD_H
#define _AD9834_CMD_H

/* Includes ------------------------------------------------------------------*/
#include "misc.h"
//#include "myString.h"
#include "AD9834.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
extern const char AD9834_HELP[];
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
u8 AD9834Cmd(AD9834_Dev_T *pDev, const char* CMD, void (*print)(const char* FORMAT_ORG, ...));
//call back reference
#endif /* _AD9834_CMD_H */

/******************* (C) COPYRIGHT 2015 INCUBECN *****END OF FILE****/
