/**********************************************************
filename: output.h
**********************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _OUTPUT_H
#define _OUTPUT_H

#include "pca9539.h"

#define OUTPUT_POLLING_TIME 16

typedef enum{
	PIN_RESET = 0,
	PIN_SET = 1
}OUTPUT_STATUS;

typedef struct {
	char name[DEV_NAME_LEN];
	PCA9539_Dev_T* pca9539[2];
	u16 status;
	u16 autoToggleTick[32];
	u16 autoToggleTmr[32];
} OUTPUT_RSRC_T;

typedef struct {	
	OUTPUT_RSRC_T rsrc;
	void (*WritePinHEX)(OUTPUT_RSRC_T* pRsrc, u16 pinStatus);
	void (*WritePin)(OUTPUT_RSRC_T* pRsrc, u8 pinIndx, OUTPUT_STATUS status);
	void (*TogglePin)(OUTPUT_RSRC_T* pRsrc, u8 pinIndx);
	s8 (*AutoTogglePin) (OUTPUT_RSRC_T* pRsrc, u8 pinIndx, u16 mSec);
	s8 (*StopTogglePin) (OUTPUT_RSRC_T* pRsrc, u8 pinIndx, u16 status);
	void (*Polling)(OUTPUT_RSRC_T *pRsrc);
}OUTPUT_DEV_T;

/* output variables for extern function --------------------------------------*/
DEV_STATUS outputDevSetup(
	OUTPUT_DEV_T *pDev, 
	PCA9539_Dev_T* pcaL,
	PCA9539_Dev_T* pcaH,
	u16 intiStatus
);
#endif
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
