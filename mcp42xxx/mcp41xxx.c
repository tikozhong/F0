/* Includes ------------------------------------------------------------------*/
#include "mcp41xxx.h"

/* Public variables ---------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void mcp41xxxWrite (MCP41XXX_Rsrc_T *pRsrc, u16 x16);
void mcp41xxx_setPos (MCP41XXX_Rsrc_T *pRsrc, u8 pos);
void mcp41xxx_shut (MCP41XXX_Rsrc_T *pRsrc);
void mcp41xxx_wakeup (MCP41XXX_Rsrc_T *pRsrc);

/* Public functions ---------------------------------------------------------*/
void MCP41XXX_Setup(MCP41XXX_Dev_T *pDev){
	MCP41XXX_Rsrc_T *pRsrc = &pDev->rsrc;
	
	HAL_GPIO_WritePin(pRsrc->nSel.GPIOx, pRsrc->nSel.GPIO_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_RESET);
	
	mcp41xxx_setPos (pRsrc, 0);
	mcp41xxx_shut (pRsrc);
	
	pDev->setPos = mcp41xxx_setPos;
	pDev->shutDown = mcp41xxx_shut;
	pDev->wakeUp = mcp41xxx_wakeup;
}

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : mcp41xxxWrite()
* Description    : 
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void mcp41xxxWrite (MCP41XXX_Rsrc_T *pRsrc, u16 x16){
	u8 i;
	u16 dat = x16;
	HAL_GPIO_WritePin(pRsrc->nSel.GPIOx, pRsrc->nSel.GPIO_Pin, GPIO_PIN_RESET);
	NOP();
	for(i=0;i<16;i++){
		//prepare data
		if(dat&0x8000)	HAL_GPIO_WritePin(pRsrc->MOSI.GPIOx, pRsrc->MOSI.GPIO_Pin, GPIO_PIN_SET);
		else HAL_GPIO_WritePin(pRsrc->MOSI.GPIOx, pRsrc->MOSI.GPIO_Pin, GPIO_PIN_RESET);
		dat <<= 1;
		NOP();
		//make rising
		HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_RESET);	NOP();NOP();NOP();NOP();
		HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_SET);		
	}
	HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_RESET);	NOP();
	HAL_GPIO_WritePin(pRsrc->nSel.GPIOx, pRsrc->nSel.GPIO_Pin, GPIO_PIN_SET);	NOP();
}

/*******************************************************************************
* Function Name  : mcp41xxx_setPos
* Description    : 
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void mcp41xxx_setPos (MCP41XXX_Rsrc_T *pRsrc, u8 pos){
	mcp41xxxWrite(pRsrc, 0x1100|pos);
	pRsrc->pos = pos;
}

/*******************************************************************************
* Function Name  : mcp41xxx_setPos
* Description    : 
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void mcp41xxx_shut (MCP41XXX_Rsrc_T *pRsrc){
	mcp41xxxWrite(pRsrc, 0x2100);
}
void mcp41xxx_wakeup (MCP41XXX_Rsrc_T *pRsrc){
	mcp41xxxWrite(pRsrc, 0x1100|(pRsrc->pos));
}

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
