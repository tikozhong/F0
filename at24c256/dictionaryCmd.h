/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : dictionaryCmd.h
* Author             : Tiko Zhong
* Date First Issued  : 09/04/2017
* Description        : This file provides a set of functions needed to manage the
*                      communication using HAL_UARTxxx
********************************************************************************
* History:
* 09/04/2017: V0.0
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _DICT_CMD_H
#define _DICT_CMD_H

/* Includes ------------------------------------------------------------------*/
#include "dictionary.h"
#include "publicRsrc.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
extern const char DICT_HELP[];
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
u8 dictionaryCmd(DictDev_T *pDev, PAKET_T *packetIn, PAKET_T *packetOut);
//call back reference

#endif /* _MAX44009_CMD_H */

/******************* (C) COPYRIGHT 2015 INCUBECN *****END OF FILE****/
