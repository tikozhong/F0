/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : cspin_command.h
* Author             : Tiko Zhong
* Date First Issued  : 12/01/2015
* Description        : This file provides a set of functions needed to manage the
*                      communication using HAL_UARTxxx
********************************************************************************
* History:
* 12/01/2015: V0.0
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _CSPIN_COMMAND_H
#define _CSPIN_COMMAND_H

/* Includes ------------------------------------------------------------------*/
#include "cspin.h"
//#include <string.h> 
#include "mystring.h"
#include "myuart.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
u8 motorCmd(UART_DEV *huartDev, const u8* cmdStr, cSPIN_DEV_T* pDev, u8 count);
void motorCmdHelp(UART_DEV *huartDev);
void motorReportAll(UART_DEV *huartDev, cSPIN_DEV_T *pDev, const u8* RtpNo);
void motorReportFew(UART_DEV *huartDev, cSPIN_DEV_T *pDev, const u8* RtpNo);

void motorReportAbsPos(UART_DEV *huartDev,const u8* leading, u32 pos);
void motorReportStatus(UART_DEV *huartDev,const u8* leading, u16 sta);
void motorReportAdcOut(UART_DEV *huartDev,const u8* leading, u8 adc);
#endif /* _CSPIN_COMMAND_H */

/******************* (C) COPYRIGHT 2015 INCUBECN *****END OF FILE****/
