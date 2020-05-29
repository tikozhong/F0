/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : ISL29125_CMD.h
* Author             : Tiko Zhong
* Date First Issued  : Jan/24/2017
* Description        : This file provides a set of functions needed to manage the
*                      communication using HAL_UARTxxx
********************************************************************************
* History:
* 24/01/2017: V0.0
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _ISL29125_CMD_H
#define _ISL29125_CMD_H

/* Includes ------------------------------------------------------------------*/
#include "misc.h"
#include "myString.h"
#include "isl29125ROM.h"
#include "publicRsrc.h"
#include "responseX.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
extern const char ISL29125_HELP[];
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
u8 ISL29125Cmd(ISL291259_Dev_T *pDev, PAKET_T *packetIn, PAKET_T *packetOut);
//call back reference
//void rgbSensorCallbackMsg(UART_DEV *huartDev, ISL291259_Dev_T* pDev);
//void rgbSensorUpdatePrnt(UART_DEV *huartDev, u8 ch, u16 val);
#endif /* _ISL29125_CMD_H */

/******************* (C) COPYRIGHT 2015 INCUBECN *****END OF FILE****/
