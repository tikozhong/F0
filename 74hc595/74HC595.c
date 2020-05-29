/**********************************************************
filename: 74hc595.c
**********************************************************/

/************************包含头文件**********************/
#include "74hc595.h"

void hc595ShftBuf (HC595RSRC_T* hc595Rsrc, u8* buff, u8 nBytes);

void hc595DevSetup(HC595DEV_T *pDev, const PIN_T LCK, const PIN_T SCK, const PIN_T DAT, HC595_FORMAT fistByte){
	pDev->rsrc.LCK = LCK;
	pDev->rsrc.SCK = SCK;
	pDev->rsrc.DAT = DAT;
	pDev->rsrc.sendFirst = fistByte;
	pDev->ShftBuf = hc595ShftBuf;	
}

/**********************************************************
 delay
**********************************************************/

/**********************************************************
 shift out data
**********************************************************/
void hc595ShftBuf (HC595RSRC_T* hc595Rsrc, u8* buff, u8 nBytes){
	u8 i,j,k;
	
	HC595RSRC_T* pRsrc = hc595Rsrc;		/* pick the param addr */
	HAL_GPIO_WritePin(pRsrc->LCK.GPIOx, pRsrc->LCK.GPIO_Pin, GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_RESET);
	NOP();NOP();
	
	for(i=0; i<nBytes; i++){
		if(pRsrc->sendFirst == LSB_FIRST)	j = buff[i];
		else	j = buff[nBytes-1-i];
		for(k=0;k<8;k++){
			if(j&0x80)	HAL_GPIO_WritePin(pRsrc->DAT.GPIOx, pRsrc->DAT.GPIO_Pin, GPIO_PIN_SET);
			else				HAL_GPIO_WritePin(pRsrc->DAT.GPIOx, pRsrc->DAT.GPIO_Pin, GPIO_PIN_RESET);
			NOP();NOP();
			HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_SET);	NOP();NOP();NOP();NOP();
			HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_RESET);
			j <<= 1;
		}
	}	
	/* latch */
	HAL_GPIO_WritePin(pRsrc->LCK.GPIOx, pRsrc->LCK.GPIO_Pin, GPIO_PIN_SET);	NOP();NOP();NOP();NOP();
	HAL_GPIO_WritePin(pRsrc->LCK.GPIOx, pRsrc->LCK.GPIO_Pin, GPIO_PIN_RESET);	NOP();NOP();NOP();NOP();
	HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(pRsrc->DAT.GPIOx, pRsrc->DAT.GPIO_Pin, GPIO_PIN_RESET);
}

/**********************************************************
 == THE END ==
**********************************************************/

