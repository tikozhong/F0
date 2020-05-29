/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : mcp41xxx.h
* Author             : Tiko Zhong
* Date First Issued  : Jun/20/2016
* Description        : To manage up to 32 task
********************************************************************************
* History:
* Jun/20/2016: V00
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MCP41XXX_H
#define _MCP41XXX_H

/* Includes ------------------------------------------------------------------*/
#include "misc.h"

/* Exported types ------------------------------------------------------------*/
typedef struct{
	const PIN_T nSel;
	const PIN_T SCK;
	const PIN_T MOSI;
	u8 pos;
}MCP41XXX_Rsrc_T;

typedef struct{
	MCP41XXX_Rsrc_T rsrc;
	void (*setPos)(MCP41XXX_Rsrc_T *pRsrc, u8 pos);
	void (*shutDown)(MCP41XXX_Rsrc_T *pRsrc);
	void (*wakeUp)(MCP41XXX_Rsrc_T *pRsrc);
}MCP41XXX_Dev_T;

/* Exported variables --------------------------------------------------------*/
//extern u8 rxBufIndx;
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void MCP41XXX_Setup(MCP41XXX_Dev_T *pDev);

#endif /* _MCP41XXX_H */

/******************* (C) COPYRIGHT 2015 INCUBECN *****END OF FILE****/
