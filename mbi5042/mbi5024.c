/**********************************************************
filename: mbi5024.c
**********************************************************/

/************************包含头文件***************************************************/
#include "mbi5024.h"
#include "publicRsrc.h"

const u16 COLOR[8]={
	0,			//off
	2,			//red
	4,			//green
	1,			//blue
	(2+4),	//yellow
	(2+1),	//purple
	(4+1),	//green-blue
	(2+4+1)	//white
};

/**********************************************************
 Private function
**********************************************************/
static void setLED(MBI5024_Rsrc_T* pRsrc, u8 portIndx, u8 ledIndx, u8 color);
/**********************************************************
 Public function
**********************************************************/
DEV_STATUS MBI5024_Setup(
	MBI5024_Dev_T *pDev, 
	PIN_T lck1, PIN_T lck2, PIN_T lck3,
	PIN_T sck, PIN_T dat
){
	MBI5024_Rsrc_T* pRsrc = &pDev->rsrc;
	memset(pRsrc,0,sizeof(MBI5024_Rsrc_T));
	//config
	pRsrc->LCK0 = lck1;
	pRsrc->LCK1 = lck2;
	pRsrc->LCK2 = lck3;
	pRsrc->SCK = sck;
	pRsrc->DAT = dat;
	PIN_OP.As_OUTPUT_PP_PULLDWN_LOW(pRsrc->LCK0);
	PIN_OP.As_OUTPUT_PP_PULLDWN_LOW(pRsrc->LCK1);
	PIN_OP.As_OUTPUT_PP_PULLDWN_LOW(pRsrc->LCK2);
	PIN_OP.As_OUTPUT_PP_PULLDWN_LOW(pRsrc->SCK);
	PIN_OP.As_OUTPUT_PP_PULLDWN_LOW(pRsrc->DAT);
	//ops
	pDev->SetLED = setLED;
	return DEV_SUCCESS;
}

/**********************************************************
 Public function
**********************************************************/
static void setLED(MBI5024_Rsrc_T* pRsrc, u8 portIndx, u8 ledIndx, u8 color){
	u16 i;
	u16 tmp16b;

	if(portIndx>=3 || color >=8 || ledIndx>=3)	return;
	
	//before shift 
	tmp16b = color;
	pRsrc->buf[portIndx] &= ((u16)0x07<<(ledIndx*3))^0xffff;
	pRsrc->buf[portIndx] |= (tmp16b<<(ledIndx*3));
	i = pRsrc->buf[portIndx];
	tmp16b = 0xfe00;
	tmp16b |= COLOR[i&0x07];		//1st led
	i >>= 3;
	tmp16b |= COLOR[i&0x07]<<3;	//2nd led
	i >>= 3;
	tmp16b |= COLOR[i&0x07]<<6;	//3rd led
	
	//shift led data
	for(i=0;i<16;i++){
		if(tmp16b&0x8000)	HAL_GPIO_WritePin(pRsrc->DAT.GPIOx, pRsrc->DAT.GPIO_Pin, GPIO_PIN_SET);
		else							HAL_GPIO_WritePin(pRsrc->DAT.GPIOx, pRsrc->DAT.GPIO_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_SET);	
		HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_RESET);
		tmp16b <<= 1;
	}
	
	switch (portIndx){
		case 0:	
			HAL_GPIO_WritePin(pRsrc->LCK0.GPIOx, pRsrc->LCK0.GPIO_Pin, GPIO_PIN_RESET);	
			HAL_GPIO_WritePin(pRsrc->LCK0.GPIOx, pRsrc->LCK0.GPIO_Pin, GPIO_PIN_SET);	NOP();NOP();NOP();NOP();
			HAL_GPIO_WritePin(pRsrc->LCK0.GPIOx, pRsrc->LCK0.GPIO_Pin, GPIO_PIN_RESET);	
			break;
		case 1:	
			HAL_GPIO_WritePin(pRsrc->LCK1.GPIOx, pRsrc->LCK1.GPIO_Pin, GPIO_PIN_RESET);	
			HAL_GPIO_WritePin(pRsrc->LCK1.GPIOx, pRsrc->LCK1.GPIO_Pin, GPIO_PIN_SET);	NOP();NOP();NOP();NOP();
			HAL_GPIO_WritePin(pRsrc->LCK1.GPIOx, pRsrc->LCK1.GPIO_Pin, GPIO_PIN_RESET);	
			break;
		case 2:	
			HAL_GPIO_WritePin(pRsrc->LCK2.GPIOx, pRsrc->LCK2.GPIO_Pin, GPIO_PIN_RESET);	
			HAL_GPIO_WritePin(pRsrc->LCK2.GPIOx, pRsrc->LCK2.GPIO_Pin, GPIO_PIN_SET);	NOP();NOP();NOP();NOP();
			HAL_GPIO_WritePin(pRsrc->LCK2.GPIOx, pRsrc->LCK2.GPIO_Pin, GPIO_PIN_RESET);			
			break;
		default: 
			break;
	}
	
}


/**********************************************************
 == THE END ==
**********************************************************/
