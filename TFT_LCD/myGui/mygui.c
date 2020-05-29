/**********************************************
filename:	myGui.c
**********************************************/
#include "myGUI.h"
#include "string.h"
#include "myuart_os.h"
#include "page00.h"

/* using external variables ------------------------------------*/
/* global variables --------------------------------------------*/
/* private variables -------------------------------------------*/

/* private Functions -------------------------------------------*/
/********************
* 
********************/
s8 guiSetup(
	guiDev_t *pDev,
	//LCD config
	PIN_T RS, 		//Resister select pin
	PIN_T CS, 		//SPI select pin
	SPI_HandleTypeDef *SPI_HANDLE,
	u16 xLen, u16 yLen
){
	rm68140Dev_T* pLCD = &pDev->rsrc.lcd; 
	page00Dev_t page0;
	
	if(Rm68140Setup(pLCD, RS, CS, SPI_HANDLE, xLen, yLen) <0 )	return -1;
	if(fontDevSetup(&pDev->rsrc.font, pLCD, WHITE, &FONT_GEORGIA16) <0 )	return -1;
	//	rm68140Dev_T *lcd, fontDev_T *font, u16 bColor, u16 actColor, u16 txtColor, u8 fontSZ
	page00Setup(&page0, pLCD, &pDev->rsrc.font, 768, 2016, 0xffff, &FONT_GEORGIA28);	//bcolor768
	
	pDev->rsrc.font.printX(&pDev->rsrc.font.rsrc, 20,40,0xffff, &FONT_CONSOLAS38, "AVG 0.10193mA" );
	pDev->rsrc.font.printX(&pDev->rsrc.font.rsrc, 20,100,0xffff, &FONT_CONSOLAS38, "MIN 12.0193uV" );
	pDev->rsrc.font.printX(&pDev->rsrc.font.rsrc, 20,160,0xffff, &FONT_CONSOLAS38, "MAX 1201.93uV" );
	pDev->rsrc.font.printX(&pDev->rsrc.font.rsrc, 20,220,0xffff, &FONT_CONSOLAS38, "DELTA 01.15%" );
	return 0;
}

