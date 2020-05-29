/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : mcp42xxx.h
* Author             : Tiko Zhong
* Date First Issued  : March/12/2020
* Description        : To manage up to 32 task
********************************************************************************
* History:
* Jun/20/2016: V00
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MCP42XXX_H
#define _MCP42XXX_H

/* Includes ------------------------------------------------------------------*/
#include "misc.h"

/* Exported types ------------------------------------------------------------*/
typedef struct{
	PIN_T nSel;
	PIN_T SCK;
	PIN_T MOSI;
	u8 posA, posB;
	char name[DEV_NAME_LEN];
}MCP42XXX_Rsrc_T;

typedef struct{
	MCP42XXX_Rsrc_T rsrc;
	void (*SetPosBoth)(MCP42XXX_Rsrc_T *pRsrc, u8 pos);
	void (*SetPosA)(MCP42XXX_Rsrc_T *pRsrc, u8 pos);
	void (*SetPosB)(MCP42XXX_Rsrc_T *pRsrc, u8 pos);
	void (*StepPosA)(MCP42XXX_Rsrc_T *pRsrc, s8 step);
	void (*StepPosB)(MCP42XXX_Rsrc_T *pRsrc, s8 step);
	void (*ShutDown)(MCP42XXX_Rsrc_T *pRsrc);
	void (*WakeUp)(MCP42XXX_Rsrc_T *pRsrc);
	void (*Rename)(MCP42XXX_Rsrc_T* pRsrc, const char* NEW_NAME);
}MCP42XXX_Dev_T;

/* Exported variables --------------------------------------------------------*/
//extern u8 rxBufIndx;
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void MCP42XXX_Setup(
	MCP42XXX_Dev_T *pDev,
	PIN_T nSel,
	PIN_T SCK,
	PIN_T MOSI
);

#endif /* _MCP42XXX_H */

/******************* (C) COPYRIGHT 2015 INCUBECN *****END OF FILE****/
