/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : mcp3421_CMD.h
* Author             : Tiko Zhong
* Date First Issued  : Jan/24/2017
* Description        : This file provides a set of functions needed to manage the
*                      communication using HAL_UARTxxx
********************************************************************************
* History:
* 24/01/2017: V0.0
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MCP3421_CMD_H
#define _MCP3421_CMD_H

/* Includes ------------------------------------------------------------------*/
#include "misc.h"
#include "myString.h"
#include "publicRsrc.h"
#include "mcp3421.h"
#include "responseX.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
extern const char MC3421_HELP[];
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
u8 mcp3421Cmd(MCP3421_Dev_T *pDev, PAKET_T *packetIn, PAKET_T *packetOut);
//call back reference
#endif /* _ISL29125_CMD_H */

/******************* (C) COPYRIGHT 2015 INCUBECN *****END OF FILE****/
