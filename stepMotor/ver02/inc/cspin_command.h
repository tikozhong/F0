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
uint8_t motorCmd(UART_RSRC *huartRsrc, uint8_t* cmdStr, cSPIN_Rsrc_T* pRsrc, uint8_t motorSum);
void motorCmdHelp(UART_RSRC *huartRsrc);

#endif /* _CSPIN_COMMAND_H */

/******************* (C) COPYRIGHT 2015 INCUBECN *****END OF FILE****/
