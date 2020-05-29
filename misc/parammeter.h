/**
 ******************************************************************************
  * @file    parammeter.h
  * @brief
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _PARAMMETER_H
#define _PARAMMETER_H

#include "misc.h"
#include "parammeter_DEF.h"

#define PARAM_IS_STR	1

typedef struct{
	u8 funName[FUN_NAME_SZ];
	void *params[PARAM_SUM];
	u32 paramU32[PARAM_SUM];
	u8 paramStr[PARAM_SUM][PARAM_STR_SZ];
	u32 format;	
	u8 indx,indxU32,indxStr;
} PARAM_RSRC_T;

typedef struct{
	PARAM_RSRC_T rsrc;
	s8 (*Comply)	(PARAM_RSRC_T* pRsrc, const u8* STR, CMD_DEF_T* DEF, u8 defLen);
} COMPLIER_DEV_T;

void setupComplier(COMPLIER_DEV_T *pDev);

#endif
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
