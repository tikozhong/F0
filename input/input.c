#include "input.h"
#include "string.h"

/**********************************************************
 private function
**********************************************************/
static void inputFetch(INPUT_RSRC_T* pRsrc);
static s8 inputReadPin(INPUT_RSRC_T* pRsrc, u8 pin);
/**********************************************************
 public function
**********************************************************/
DEV_STATUS InputDevSetup(
	INPUT_DEV_T *pDev, 
	PCA9539_Dev_T* pcaL,
	PCA9539_Dev_T* pcaH
){
	INPUT_RSRC_T *pRsrc = &pDev->rsrc;
	memset(pRsrc, 0, sizeof(INPUT_RSRC_T));
	pRsrc->pca9539[0] = pcaL;
	pRsrc->pca9539[1] = pcaH;
	pRsrc->status = 0xffff;
	pDev->Fetch = inputFetch;
	pDev->ReadPin = inputReadPin;
	return DEV_SUCCESS;
}

/*******************************************************************************
* Function Name  : inputFetch
* Description    : per 4ms timer call back, do inputFetch
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void inputFetch(INPUT_RSRC_T* pRsrc){
	u8 i,j,k;	u32 edge;
	PCA9539_Dev_T* pca9539;
	
	pca9539 = pRsrc->pca9539[0];
	pca9539->ReadReg(&pca9539->rsrc, 0, &i, 1);	
	pca9539 = pRsrc->pca9539[1];
	pca9539->ReadReg(&pca9539->rsrc, 0, &j, 1);
	//shift in ram
	for(k=0;k<8;k++){
		pRsrc->series[k] <<= 1;
		if(i&0x01)	pRsrc->series[k] |= 0x00000001;
		i >>= 1;
	}
	for(k=8;k<16;k++){
		pRsrc->series[k] <<= 1;
		if(j&0x01)	pRsrc->series[k] |= 0x00000001;
		j >>= 1;
	}

	//update status and generate callback msg
	pRsrc->fallingEvt = 0;
	pRsrc->raisingEvt = 0;
	for(i=0; i<16; i++)	{
		//make all inputs' statue in 32-bit
		if((pRsrc->series[i]&0x0000000f) == 0)	pRsrc->status &= (1U<<i)^0xffff;
		else if((pRsrc->series[i]&0x0000000f) == 0x0f)	pRsrc->status |= 1U<<i;
		//deal with edge event
		edge = pRsrc->series[i];
		edge &= 0xf00f;
		if((edge == 0xf000) && (pRsrc->enableFalling & (1U<<i))){
			pRsrc->fallingEvt |= 1U<<i;
			pRsrc->series[i] &= 0xfffff000;	//in case of one event responsing two times
		}
		if((edge == 0x000f) && (pRsrc->enableRaising & (1U<<i))){
			pRsrc->raisingEvt |= 1U<<i;
			pRsrc->series[i] |= 0x0000ffff;	//in case of one event responsing two times
		}
	}
}

/*******************************************************************************
* Function Name  : InputReadPin
* Description    : all sample 
* Input          : None
* Output         : 0: low, 1:high	2:shaking
* Return         : None
*******************************************************************************/
static s8 inputReadPin(INPUT_RSRC_T* pRsrc, u8 pin){
	if(pRsrc->status & (1U<<pin))	return 1;
	else return 0;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
