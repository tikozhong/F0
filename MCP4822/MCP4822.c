/**********************************************************
filename: mcp4822.c
**********************************************************/

/*********************************************************/
#include "mcp4822.h"

/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private variables ------------------------------------*/
/* Private function prototypes --------------------------*/
static void setVolt_mV	(MCP4822Rsrc_T* pRsrc, u8 ch, u16 mv);
static void setVolt_500uV	(MCP4822Rsrc_T* pRsrc, u8 ch, u16 _500uV);
static void shift(MCP4822Rsrc_T* pRsrc, u16 config);
static void GPIO_PP(PIN_T X, GPIO_PinState);
/**********************************************************
 Public function
**********************************************************/
s8 mcp4822DevSetup(
	MCP4822Dev_t *pDev, 
	PIN_T CS, 
	PIN_T SCK,
	PIN_T SDI
){
	MCP4822Rsrc_T *pRsrc = &pDev->rsrc;
	//resource
	pRsrc->CS = CS;
	pRsrc->SCK = SCK;
	pRsrc->SDI = SDI;
	//methods
	pDev->SetVolt_500uV = setVolt_500uV;
	pDev->SetVolt_mV = setVolt_mV;
	//config gpio
	GPIO_PP(CS, MCP4822_SET);
	GPIO_PP(SCK, MCP4822_RESET);
	GPIO_PP(SDI, MCP4822_RESET);	
	//shut down
	setVolt_500uV(pRsrc,0,0);
	NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
	setVolt_500uV(pRsrc,1,0);
	return 0;
}

//apply layer
/**********************************************************
* Function Name  : setVolt_mV
* Description    : 
* Input          : None
* Output         : None
* Return         : None
**********************************************************/
static void setVolt_mV	(MCP4822Rsrc_T* pRsrc, u8 ch, u16 mV){
	setVolt_500uV(pRsrc, ch, mV*2);
}

static void setVolt_500uV	(MCP4822Rsrc_T* pRsrc, u8 ch, u16 _500uV){
	u16 cfg = _500uV & 0x0fff;
	//set ch
	if(ch)	cfg |= BIT15;
	if(_500uV == 0){	//shun down
		shift(pRsrc, cfg);
	}
	else if(_500uV >= 2048*2){	//Gain = x2
		cfg |= BIT12;	//open
		shift(pRsrc, cfg);
	}
	else{	//Gain = x1
		cfg |= BIT12;	//open
		cfg |= BIT13;		
		shift(pRsrc, cfg);
	}
}

//low driver layer
static void shift(MCP4822Rsrc_T* pRsrc, u16 config){
	u8 i;
	HAL_GPIO_WritePin(pRsrc->CS.GPIOx, pRsrc->CS.GPIO_Pin, MCP4822_SET);	
	NOP();NOP();NOP();NOP();
	HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, MCP4822_RESET);
	HAL_GPIO_WritePin(pRsrc->CS.GPIOx, pRsrc->CS.GPIO_Pin, MCP4822_RESET);	
	for(i=0;i<16;i++){
		if(config & BIT15)	HAL_GPIO_WritePin(pRsrc->SDI.GPIOx, pRsrc->SDI.GPIO_Pin, MCP4822_SET);	
		else	HAL_GPIO_WritePin(pRsrc->SDI.GPIOx, pRsrc->SDI.GPIO_Pin, MCP4822_RESET);	
		HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, MCP4822_SET);
		config <<= 1;
		HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, MCP4822_RESET);
	}
	HAL_GPIO_WritePin(pRsrc->CS.GPIOx, pRsrc->CS.GPIO_Pin, MCP4822_SET);	
}

static void GPIO_PP(PIN_T X, GPIO_PinState LEVEL){
	 GPIO_InitTypeDef GPIO_InitStruct;	
  /*Configure GPIO pins : pRsrc->SDA.GPIO_Pin */
	HAL_GPIO_WritePin(X.GPIOx, X.GPIO_Pin, LEVEL);
  GPIO_InitStruct.Pin = X.GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(X.GPIOx, &GPIO_InitStruct);	
}

/**********************************************************
 == THE END ==
**********************************************************/
