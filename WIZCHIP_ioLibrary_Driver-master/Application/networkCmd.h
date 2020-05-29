/******************** (C)  2015 INCUBECN *****************************
* File Name          : networkCmd.h
* Author             : Tiko Zhong
* Date First Issued  : 12/01/2015
* Description        : This file provides a set of functions needed to manage the
*                      communication using HAL_UARTxxx
********************************************************************************
* History:
* 12/01/2015: V0.0
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _NETWORK_CMD_H
#define _NETWORK_CMD_H

/* Includes ------------------------------------------------------------------*/
//#include "misc.h"
#include "publicRsrc.h"
#include "W5500X.h"

/* Exported types ------------------------------------------------------------*/
typedef enum{
	INPUT_EVENT_FALLING	=	0,
	INPUT_EVENT_RAISING	=	1,
}NETWORK_EVENT_T;
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
extern const u8 NETWORK_HELP[];

/* Exported functions ------------------------------------------------------- */
u8 networkCmd(W5500_Dev_T *pDev, PAKET_T *packetIn, PAKET_T *packetOut);
void networkMakeEventMsg(PAKET_T *packetOut, const char* DevName, u8 pinIndx, NETWORK_EVENT_T edge);
#endif /* _INPUT_CMD_H */

/******************* (C) COPYRIGHT 2015 INCUBECN *****END OF FILE****/
