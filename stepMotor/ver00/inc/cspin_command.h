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
#include "stm32f0xx_hal.h"
#include "cspin.h"
/* Exported types ------------------------------------------------------------*/
/* cSPIN internal register addresses */
typedef enum {
	Cmd_PrtDis =0,	
	Cmd_PrtEn = 1
}cSPIN_Cmd_PrtEn_T;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint8_t cspin_command(UART_HandleTypeDef *huart, cSPIN_Rsrc_T* pRsrc,uint8_t* str, cSPIN_Cmd_PrtEn_T printEn);

#endif /* _CSPIN_COMMAND_H */

/******************* (C) COPYRIGHT 2015 INCUBECN *****END OF FILE****/
