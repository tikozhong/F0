/**********************************************************
filename: ad9834.h
**********************************************************/
#ifndef _AD9834_H_
#define _AD9834_H_

#include "misc.h"

#define ADC_BUF_LEN 8

/*****************************************************************************
 @ typedefs
****************************************************************************/
typedef struct{
	char name[DEV_NAME_LEN];
	/* config	*/
	PIN_T SCLK,SDAT,FSYNC;
	PIN_T RESET,SLEEP,PSEL,FSEL;
	u8 msgEn;
	u8 frqRegSel, phsRegSel;
	u32 uHzPerLsb;
	u32 curFrq, endFrq;
	s32 stepFrq;
	u16 stepTime;
	u8 squ, tmr, tick;
	u8 errCode;
	char callBackMsg[CALLBACK_MSG_LEN];
	//delegrate
	void (*print)(const char* FORMAT_ORG, ...);
	void (*DelayMs)			(u8 ms);
}AD9834_Rsrc_T;

typedef struct{
	AD9834_Rsrc_T rsrc;
	//usr operation
	void (*Rename)(AD9834_Rsrc_T* pRsrc, const char* NEW_NAME);
	void (*SineWave)(AD9834_Rsrc_T* pRsrc, u32 frq_mHz);
	void (*TriangleWave)(AD9834_Rsrc_T* pRsrc, u32 frq_mHz);
	void (*ScanSineWave)(AD9834_Rsrc_T* pRsrc, u32 startFrq, u32 endFrq, s32 stepFrq, u16 stepTim);
	void (*ScanTriangleWave)(AD9834_Rsrc_T* pRsrc, u32 startFrq, u32 endFrq, s32 stepFrq, u16 stepTim);
	void (*Powerdown)(AD9834_Rsrc_T* pRsrc);
	void (*Polling)(AD9834_Rsrc_T* pRsrc, u8 tick);
	//Lower operation
	void (*WriteReg)(AD9834_Rsrc_T* pRsrc, u16 reg);
}AD9834_Dev_T;

s8 AD9834DevSetup(
	AD9834_Dev_T *pDev, 
	PIN_T SCLK,
	PIN_T SDATA,
	PIN_T FSYNC,
	PIN_T RESET,
	u8 MCLK_MHZ,
	void (*print)(const char* FORMAT_ORG, ...)
);

#endif
