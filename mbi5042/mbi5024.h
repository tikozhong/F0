/**********************************************************
filename: mbi5024.h
**********************************************************/
#ifndef _MBI5024_H_
#define _MBI5024_H_

#include "misc.h"
/*****************************************************************************
 @ typedefs
****************************************************************************/
typedef struct{
	char name[DEV_NAME_LEN];
	//config
	PIN_T LCK0,LCK1,LCK2,SCK,DAT;
	u16 buf[3];
}MBI5024_Rsrc_T;

typedef struct{
	//resource
	MBI5024_Rsrc_T rsrc;
	//operation
	void (*SetLED)(MBI5024_Rsrc_T* pRsrc, u8 portIndx, u8 ledIndx, u8 color);
}MBI5024_Dev_T;

DEV_STATUS MBI5024_Setup(
	MBI5024_Dev_T *pDev, 
	PIN_T lck1, PIN_T lck2, PIN_T lck3,
	PIN_T sck, PIN_T dat
);
#endif
