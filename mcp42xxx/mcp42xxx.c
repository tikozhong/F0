/* Includes ------------------------------------------------------------------*/
#include "mcp42xxx.h"
#include "string.h"
#include "gpiodecal.h"

/* Public variables ---------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void mcp42xxxWrite (MCP42XXX_Rsrc_T *pRsrc, u16 x16);
static void mcp42xxx_setPosBoth (MCP42XXX_Rsrc_T *pRsrc, u8 pos);
static void mcp42xxx_setPosA (MCP42XXX_Rsrc_T *pRsrc, u8 pos);
static void mcp42xxx_setPosB (MCP42XXX_Rsrc_T *pRsrc, u8 pos);
static void mcp42xxxStepPosA(MCP42XXX_Rsrc_T *pRsrc, s8 step);
static void mcp42xxxStepPosB(MCP42XXX_Rsrc_T *pRsrc, s8 step);

static void mcp42xxx_shut (MCP42XXX_Rsrc_T *pRsrc);
static void mcp42xxx_wakeup (MCP42XXX_Rsrc_T *pRsrc);
static void mcp42xxxRename(MCP42XXX_Rsrc_T* pRsrc, const char* NEW);
/* Public functions ---------------------------------------------------------*/
void MCP42XXX_Setup(
	MCP42XXX_Dev_T *pDev,
	PIN_T nSel,
	PIN_T SCK,
	PIN_T MOSI
){
	MCP42XXX_Rsrc_T *pRsrc = &pDev->rsrc;
	
	pRsrc->nSel = nSel;
	pRsrc->SCK = SCK;
	pRsrc->MOSI = MOSI;
	
	HAL_GPIO_WritePin(pRsrc->nSel.GPIOx, pRsrc->nSel.GPIO_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_RESET);
	
	mcp42xxx_shut (pRsrc);
	
	pDev->SetPosA = mcp42xxx_setPosA;
	pDev->SetPosB = mcp42xxx_setPosB;
	pDev->SetPosBoth = mcp42xxx_setPosBoth;
	pDev->StepPosA = mcp42xxxStepPosA;
	pDev->StepPosB = mcp42xxxStepPosB;
	pDev->ShutDown = mcp42xxx_shut;
	pDev->WakeUp = mcp42xxx_wakeup;
	pDev->Rename = mcp42xxxRename;
}

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : mcp42xxxWrite()
* Description    : 
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
static void mcp42xxxWrite (MCP42XXX_Rsrc_T *pRsrc, u16 x16){
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
* Function Name  : mcp42xxx_setPos
* Description    : 
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
static void mcp42xxx_setPosBoth (MCP42XXX_Rsrc_T *pRsrc, u8 pos){
	//xx01 xx P1P0 data
	mcp42xxxWrite(pRsrc, 0x1300|pos);
	pRsrc->posA = pos;
	pRsrc->posB = pos;
}

static void mcp42xxx_setPosA (MCP42XXX_Rsrc_T *pRsrc, u8 pos){
	//xx10 xxP1P0 data
	mcp42xxxWrite(pRsrc, 0x1100|pos);
	pRsrc->posA = pos;
}

static void mcp42xxx_setPosB (MCP42XXX_Rsrc_T *pRsrc, u8 pos){
	//xx10 xx P1P0 data
	mcp42xxxWrite(pRsrc, 0x1200|pos);
	pRsrc->posB = pos;
}


/*******************************************************************************
* Function Name  : mcp42xxx_setPos
* Description    : 
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
static void mcp42xxx_shut (MCP42XXX_Rsrc_T *pRsrc){
	mcp42xxxWrite(pRsrc, 0x2300);
}
static void mcp42xxx_wakeup (MCP42XXX_Rsrc_T *pRsrc){
	mcp42xxx_setPosA(pRsrc, pRsrc->posA);
	mcp42xxx_setPosB(pRsrc, pRsrc->posB);
}

static void mcp42xxxRename(MCP42XXX_Rsrc_T* pRsrc, const char* NEW){
	memset(pRsrc->name, 0, DEV_NAME_LEN);
	strcpy(pRsrc->name, NEW);
}

static void mcp42xxxStepPosA(MCP42XXX_Rsrc_T *pRsrc, s8 step){
	if(pRsrc->posA+step >= 0xff)	mcp42xxx_setPosA(pRsrc, 0xff);
	else if(pRsrc->posA+step <= 0)	mcp42xxx_setPosA(pRsrc, 0);
	else mcp42xxx_setPosA(pRsrc, pRsrc->posA+step);
	
//	if(step > 0){
//		if(pRsrc->posA+step >= 0xff)	mcp42xxx_setPosA(pRsrc, 0xff);
//		else mcp42xxx_setPosA(pRsrc, pRsrc->posA+step);
//	}
//	else{
//		if(pRsrc->posA+step <= 0)	mcp42xxx_setPosA(pRsrc, 0);
//		else mcp42xxx_setPosA(pRsrc, pRsrc->posA+step);
//	}
}

static void mcp42xxxStepPosB(MCP42XXX_Rsrc_T *pRsrc, s8 step){
	if(pRsrc->posB+step >= 0xff)	mcp42xxx_setPosB(pRsrc, 0xff);
	else if(pRsrc->posB+step <= 0)	mcp42xxx_setPosB(pRsrc, 0);
	else mcp42xxx_setPosB(pRsrc, pRsrc->posB+step);
}

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
