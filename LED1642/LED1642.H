/**********************************************************
filename: led1642.h
**********************************************************/
#ifndef _LED1642_H_
#define _LED1642_H_

//#include "publicRsrc.h"
#include "misc.h"

/*****************************************************************************
 @ typedefs
****************************************************************************/
typedef struct{
	/* config	*/
	PIN_T LE,SCK,SDI;
	u16 configReg;
	u8 resolution;
	u16 brightness[16];
}LED1642_Rsrc_T;

/*
	when page write more than 64 bytes,
	pBuf must point to a static pointer
*/
typedef struct{
	LED1642_Rsrc_T rsrc;
	//op
	void (*SetGain)	(LED1642_Rsrc_T* pRsrc, u8 gain);
	s8 (*SetBrightness)	(LED1642_Rsrc_T* pRsrc, u8 ch, u16 brightness);
	s8 (*GetBrightness)	(LED1642_Rsrc_T* pRsrc, u8 ch, u16* pBrightness);
	void (*SetAllBrightness)	(LED1642_Rsrc_T* pRsrc, u16 brightness);
	void (*Switch)	(LED1642_Rsrc_T* pRsrc);
	void (*SetResolution)(LED1642_Rsrc_T* pRsrc, u8);
	void (*SetConfig)	(LED1642_Rsrc_T* pRsrc, u16 config);
	void (*GetConfig)	(LED1642_Rsrc_T* pRsrc, u16* config);
}LED1642_Dev_T;

DEV_STATUS LED1642_Setup(LED1642_Dev_T *pDev, const PIN_T LE, const PIN_T SCK, const PIN_T SDI);
#endif
