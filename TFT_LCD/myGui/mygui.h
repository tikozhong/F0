/**********************************************
filename:	myGui.h
**********************************************/
#ifndef _MY_GUI_H
#define _MY_GUI_H

#include "fontDev.h"
#include "rm68042.h"
/*****************************************************************************
 @ public defines
****************************************************************************/


/*****************************************************************************
 @ typedefs
****************************************************************************/
typedef struct {
	rm68140Dev_T lcd;
	fontDev_T font;
	
	
}guiRsrc_T;

typedef struct {
	guiRsrc_T rsrc;
	//ops
}guiDev_t;

s8 guiSetup(
	guiDev_t *pDev,
	//LCD config
	PIN_T RS, 		//Resister select pin
	PIN_T CS, 		//SPI select pin
	SPI_HandleTypeDef *SPI_HANDLE,
	u16 xLen, u16 yLen
);

#endif
