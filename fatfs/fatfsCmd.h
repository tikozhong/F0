/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : FatfsCmd.h
* Author             : Tiko Zhong
* Date First Issued  : June/24/2016
* Description        : This file provides a set of functions needed to manage the
*                      communication using HAL_UARTxxx
********************************************************************************
* History:
* 12/01/2015: V0.0
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _FATFS_CMD_H
#define _FATFS_CMD_H

/* Includes ------------------------------------------------------------------*/
#include "publicRsrc.h"
#include "sysStorage.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
extern const char FS_HELP[];
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
u8 FatfsCmd(FILE_DEV_T *pDev, PAKET_T *packetIn, PAKET_T *packetOut);

#endif /* _MCP41XXX_CMD_H */

/******************* (C) COPYRIGHT 2015 INCUBECN *****END OF FILE****/
