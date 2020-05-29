/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : MCP4822_CMD.h
* Author             : Tiko Zhong
* Date First Issued  : Jan/24/2017
* Description        : This file provides a set of functions needed to manage the
*                      communication using HAL_UARTxxx
********************************************************************************
* History:
* 24/01/2017: V0.0
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MCP4822_CMD_H
#define _MCP4822_CMD_H

/* Includes ------------------------------------------------------------------*/

#include "mcp4822.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
extern const u8 MCP4822_HELP[];
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
u8 mcp4822Cmd(MCP4822Dev_t *pDev, const u8 *cmdStr, PAKET_T *Content);
//call back reference

#endif /* _MAX44009_CMD_H */

/******************* (C) COPYRIGHT 2015 INCUBECN *****END OF FILE****/
