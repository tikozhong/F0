/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : mcp41xxxCmd.h
* Author             : Tiko Zhong
* Date First Issued  : June/21/2016
* Description        : This file provides a set of functions needed to manage the
*                      communication using HAL_UARTxxx
********************************************************************************
* History:
* 12/01/2015: V0.0
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MCP41XXX_CMD_H
#define _MCP41XXX_CMD_H

/* Includes ------------------------------------------------------------------*/
#include "misc.h"
#include "myuart.h"
#include "myString.h"
#include "mcp41xxx.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void mcp41xxxCmdHelp(UART_DEV *huartDev);
uint8_t mcp41xxxCmd(UART_DEV *huartDev, const u8* cmdStr, MCP41XXX_Dev_T *pDev);
#endif /* _MCP41XXX_CMD_H */

/******************* (C) COPYRIGHT 2015 INCUBECN *****END OF FILE****/
