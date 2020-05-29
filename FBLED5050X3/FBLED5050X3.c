/**********************************************************
filename: FBLED5050X3.c
**********************************************************/

/************************包含头文件***************************************************/
#include "FBLED5050X3.h"
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
static void setLED(FBLED5050X3_Rsrc_T* pRsrc, u8 portIndx, u8 ledIndx, u8 color);
/**********************************************************
 Public function
**********************************************************/
DEV_STATUS FBLED5050X3_Setup(
	FBLED5050X3_Dev_T *pDev, 
	const DEV_PORT* PORT
){
	FBLED5050X3_Rsrc_T* pRsrc = &pDev->rsrc;
	memset(pRsrc,0,sizeof(FBLED5050X3_Rsrc_T));
	//config
	pRsrc->PORT = PORT;
	PIN_OP.As_OUTPUT_PP_PULLDWN_LOW(PORT->PA0);
	PIN_OP.As_OUTPUT_PP_PULLDWN_LOW(PORT->PA1);
	PIN_OP.As_OUTPUT_PP_PULLDWN_LOW(PORT->PA2);
	//ops
	pDev->SetLED = setLED;
	return DEV_SUCCESS;
}

/**********************************************************
 Public function
**********************************************************/
static void setLED(FBLED5050X3_Rsrc_T* pRsrc, u8 portIndx, u8 ledIndx, u8 color){
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
//	for(i=0;i<16;i++){
//		if(tmp16b&0x8000)	HAL_GPIO_WritePin(pRsrc->DAT.GPIOx, pRsrc->DAT.GPIO_Pin, GPIO_PIN_SET);
//		else							HAL_GPIO_WritePin(pRsrc->DAT.GPIOx, pRsrc->DAT.GPIO_Pin, GPIO_PIN_RESET);
//		HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_SET);	
//		HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_RESET);
//		tmp16b <<= 1;
//	}
//	
//	switch (portIndx){
//		case 0:	
//			HAL_GPIO_WritePin(pRsrc->LCK0.GPIOx, pRsrc->LCK0.GPIO_Pin, GPIO_PIN_RESET);	
//			HAL_GPIO_WritePin(pRsrc->LCK0.GPIOx, pRsrc->LCK0.GPIO_Pin, GPIO_PIN_SET);	NOP();NOP();NOP();NOP();
//			HAL_GPIO_WritePin(pRsrc->LCK0.GPIOx, pRsrc->LCK0.GPIO_Pin, GPIO_PIN_RESET);	
//			break;
//		case 1:	
//			HAL_GPIO_WritePin(pRsrc->LCK1.GPIOx, pRsrc->LCK1.GPIO_Pin, GPIO_PIN_RESET);	
//			HAL_GPIO_WritePin(pRsrc->LCK1.GPIOx, pRsrc->LCK1.GPIO_Pin, GPIO_PIN_SET);	NOP();NOP();NOP();NOP();
//			HAL_GPIO_WritePin(pRsrc->LCK1.GPIOx, pRsrc->LCK1.GPIO_Pin, GPIO_PIN_RESET);	
//			break;
//		case 2:	
//			HAL_GPIO_WritePin(pRsrc->LCK2.GPIOx, pRsrc->LCK2.GPIO_Pin, GPIO_PIN_RESET);	
//			HAL_GPIO_WritePin(pRsrc->LCK2.GPIOx, pRsrc->LCK2.GPIO_Pin, GPIO_PIN_SET);	NOP();NOP();NOP();NOP();
//			HAL_GPIO_WritePin(pRsrc->LCK2.GPIOx, pRsrc->LCK2.GPIO_Pin, GPIO_PIN_RESET);			
//			break;
//		default: 
//			break;
//	}
	
}


/**********************************************************
 == THE END ==
**********************************************************/
