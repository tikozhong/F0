/**********************************************************
filename: input.h
**********************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _INPUT_H
#define _INPUT_H

#include "pca9539.h"

typedef struct {
	char name[DEV_NAME_LEN];
	PCA9539_Dev_T* pca9539[2];
	u16 status;
	u32 series[16];
	u16 enableFalling;
	u16 enableRaising;
	u16 fallingEvt;
	u16 raisingEvt;
} INPUT_RSRC_T;

typedef struct {
	INPUT_RSRC_T rsrc;
	void (*Fetch) (INPUT_RSRC_T* pRsrc);
	s8 (*ReadPin)	(INPUT_RSRC_T* pRsrc, u8 pin);
}INPUT_DEV_T;

/* output variables for extern function --------------------------------------*/
DEV_STATUS InputDevSetup(
	INPUT_DEV_T *pDev, 
	PCA9539_Dev_T* pcaL,
	PCA9539_Dev_T* pcaH
);

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
