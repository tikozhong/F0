/**********************************************************
filename: dac121c085.h
**********************************************************/
#ifndef _AD7793_H_
#define _AD7793_H_

#include "misc.h"

/*****************************************************************************
 @ typedefs
****************************************************************************/
typedef struct{
	/* config	*/
	PIN_T CS;
	PIN_T SCK; 
	PIN_T MISO; 
	PIN_T MOSI;
     //reg copy
     u8 clkCfg,vefVol;
     u8 tick,squ;
     u8 exitContinueRead;
     void (*printMsg)(s32 adcVal);
}AD7793_Rsrc_T;

/*
	when page write more than 64 bytes,
	pBuf must point to a static pointer
*/
typedef struct{
	AD7793_Rsrc_T rsrc;
     //API
     void (*Reset)(AD7793_Rsrc_T*);
     s8 (*StartStrm)(AD7793_Rsrc_T *pRsrc, u8 ch, u8 gain, u8 pol, u8 fs, u16 times);
     void (*Stop)(AD7793_Rsrc_T*);
     void (*Polling)(AD7793_Rsrc_T*, u8);
     //mid driver

     u8 (*ReadID)    (AD7793_Rsrc_T*);
     u8 (*ReadStatus)(AD7793_Rsrc_T*);
     u16 (*ReadMode) (AD7793_Rsrc_T*);
     void (*WriteMode) (AD7793_Rsrc_T*, u16);
     u16 (*ReadConfig) (AD7793_Rsrc_T*);
     void (*WriteConfig) (AD7793_Rsrc_T*, u16 );
     u32 (*ReadData) (AD7793_Rsrc_T*);
}AD7793_Dev_T;

DEV_STATUS AD7793_Setup(
	AD7793_Dev_T *pDev, 
	const PIN_T CS, 
	const PIN_T SCK, 
	const PIN_T MOSI, 
	const PIN_T MISO,
	u8 clkSel,
	u8 refSel
);
	
#endif
