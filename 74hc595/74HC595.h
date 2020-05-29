/**********************************************************
filename: 74hc595.h
**********************************************************/
#ifndef _74HC595_H_
#define _74HC595_H_

#include "publicRsrc.h"

#define HC595_MAX_CNT 4
/*****************************************************************************
 @ typedefs
****************************************************************************/
typedef enum{
	LSB_FIRST = 0,	//buff[0] first, buff[N-1] last
	MSB_FIRST = 1,	//buff[N-1] first, buff[0] last
}HC595_FORMAT;

typedef struct{
	PIN_T LCK;
	PIN_T SCK;
	PIN_T DAT;
	u8 sendFirst;
}HC595RSRC_T;

typedef struct{
	HC595RSRC_T rsrc;
	void (*ShftBuf)(HC595RSRC_T* hc595Rsrc, u8* buff, u8 nBytes);
} HC595DEV_T;

void hc595DevSetup(HC595DEV_T *pDev, const PIN_T LCK, const PIN_T SCK, const PIN_T DAT, HC595_FORMAT fistByte);
#endif

