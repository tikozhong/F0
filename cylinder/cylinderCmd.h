/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : cylinderCmd.h
* Author             : Tiko Zhong
* Date First Issued  : 12/01/2015
* Description        : This file provides a set of functions needed to manage the
*                      communication using HAL_UARTxxx
********************************************************************************
* History:
* 12/01/2015: V0.0
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _CYLINDER_CMD_H
#define _CYLINDER_CMD_H

/* Includes ------------------------------------------------------------------*/
#include "cylinder.h"
#include "publicRsrc.h"
/* Exported types ------------------------------------------------------------*/
extern const char CLDR_HELP[];
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
u8 cylinderCmd(CLDR_DEV_T* pDev, PAKET_T *packetIn, PAKET_T *packetOut);
//void cylinderCallbackMsg(UART_DEV *huartDev, CLDR_DEV_T* pDev);

#endif /* _B238_CMD_H */

/******************* (C) COPYRIGHT 2015 INCUBECN *****END OF FILE****/
