/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : mcp42xxx_CMD.h
* Author             : Tiko Zhong
* Date First Issued  : mar/12/2020
* Description        : This file provides a set of functions needed to manage the
*                      communication using HAL_UARTxxx
********************************************************************************
* History:
* march/12/2020: V0.0
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MCP42XXX_CMD_H
#define _MCP42XXX_CMD_H

/* Includes ------------------------------------------------------------------*/
#include "misc.h"
//#include "myString.h"
#include "mcp42xxx.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
extern const char MCP42XXX_HELP[];
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
u8 MCP42XXX_Cmd(MCP42XXX_Dev_T *pDev, const char* CMD, void (*print)(const char* FORMAT_ORG, ...));
//call back reference
#endif /* _AD9834_CMD_H */

/******************* (C) COPYRIGHT 2015 INCUBECN *****END OF FILE****/
