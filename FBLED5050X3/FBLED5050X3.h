/**********************************************************
filename: FBLED5050X3.h
**********************************************************/
#ifndef _FBLED5050X3_H_
#define _FBLED5050X3_H_

#include "misc.h"
/*****************************************************************************
 @ typedefs
****************************************************************************/
typedef struct{
	char name[DEV_NAME_LEN];
	//config
	const DEV_PORT* PORT;
	u16 buf[3];
}FBLED5050X3_Rsrc_T;

typedef struct{
	//resource
	FBLED5050X3_Rsrc_T rsrc;
	//operation
	void (*SetLED)(FBLED5050X3_Rsrc_T* pRsrc, u8 portIndx, u8 ledIndx, u8 color);
}FBLED5050X3_Dev_T;

DEV_STATUS FBLED5050X3_Setup(
	FBLED5050X3_Dev_T *pDev, 
	const DEV_PORT* PORT
);
#endif
