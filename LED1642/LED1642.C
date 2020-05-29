/**********************************************************
filename: led1642.c
**********************************************************/

/************************包含头文件***************************************************/
#include "led1642.h"
#include "publicRsrc.h"

#define TW_MS 5
#define MAX_PAGE_SZ	128

/**********************************************************
 Private function
**********************************************************/
static void setMaxCurrent	(LED1642_Rsrc_T* pRsrc, u8 maxCurrent);
static s8 setBrightness	(LED1642_Rsrc_T* pRsrc, u8 ch, u16 brightness);
static s8 getBrightness	(LED1642_Rsrc_T* pRsrc, u8 ch, u16* pBrightness);
static void setAllBrightness	(LED1642_Rsrc_T* pRsrc, u16 brightness);
static void toggle(LED1642_Rsrc_T* pRsrc);
static void setConfig	(LED1642_Rsrc_T* pRsrc, u16 config);
static void getConfig	(LED1642_Rsrc_T* pRsrc, u16* config);
static void setResolution	(LED1642_Rsrc_T* pRsrc, u8 res);

static void led1642UpdateBrightness(LED1642_Rsrc_T* pRsrc);
static void led1642Shift(LED1642_Rsrc_T* pRsrc, u16 data, u8 leUpIndx);
static u16 led1642Read(LED1642_Rsrc_T* pRsrc);
/**********************************************************
 Public function
**********************************************************/
DEV_STATUS LED1642_Setup(LED1642_Dev_T *pDev, const PIN_T LE, const PIN_T SCK, const PIN_T SDI){
	LED1642_Rsrc_T* pRsrc = &pDev->rsrc;
	pRsrc->LE = LE;
	pRsrc->SCK = SCK;
	pRsrc->SDI = SDI;
	//gpio config
	gpioConfig.As_OUTPUT_PP_PULLDWN_LOW(LE.GPIOx, LE.GPIO_Pin);
	gpioConfig.As_OUTPUT_PP_PULLDWN_LOW(SCK.GPIOx, SCK.GPIO_Pin);
	gpioConfig.As_OUTPUT_PP_PULLDWN_LOW(SDI.GPIOx, SDI.GPIO_Pin);
	//chip initial
	setMaxCurrent(pRsrc, 0x7f);
	setResolution(pRsrc,0);
	pRsrc->resolution = 0;	//12bit	
	setAllBrightness(pRsrc, 1);
	//op
	pDev->GetBrightness = getBrightness;
	pDev->SetAllBrightness = setAllBrightness;
	pDev->SetBrightness = setBrightness;
	pDev->SetGain = setMaxCurrent;
	pDev->Switch = toggle;
	pDev->SetConfig = setConfig;
	pDev->GetConfig = getConfig;
	pDev->SetResolution = setResolution;
	return DEV_SUCCESS;
}

/**********************************************************
 Public function
**********************************************************/
static void setMaxCurrent	(LED1642_Rsrc_T* pRsrc, u8 maxCurrent){
	pRsrc->configReg =  (pRsrc->configReg & (0x7f^0xffff)) | (maxCurrent & 0x7f);
	pRsrc->configReg |= 0x8000;
	led1642Shift(pRsrc, pRsrc->configReg, (16-7));		//config reg latch, LE keep high for last 7 sck
}

static s8 setBrightness	(LED1642_Rsrc_T* pRsrc, u8 ch, u16 brightness){
	if(ch>=16)	return -1;
	pRsrc->brightness[ch] = brightness;
	led1642UpdateBrightness(pRsrc);
	return 0;
}

void setAllBrightness	(LED1642_Rsrc_T* pRsrc, u16 brightness){
	u8 i;
	for(i=0;i<16;i++)		pRsrc->brightness[i] = brightness;
	led1642UpdateBrightness(pRsrc);
}

static s8 getBrightness	(LED1642_Rsrc_T* pRsrc, u8 ch, u16* pBrightness){
	if(ch>=16 || pBrightness==NULL)	return -1;
	*pBrightness = pRsrc->brightness[ch];
	return 0;
}

static void led1642UpdateBrightness(LED1642_Rsrc_T* pRsrc){
	u8 i;
	for(i=0;i<15;i++)
		led1642Shift(pRsrc, pRsrc->brightness[15-i],(16-4));	//data latch
	led1642Shift(pRsrc, pRsrc->brightness[0],(16-6));				//global latch
}

static void toggle(LED1642_Rsrc_T* pRsrc){
	led1642Shift(pRsrc, 0xffff,(16-1));
}

static void setConfig	(LED1642_Rsrc_T* pRsrc, u16 config){
	pRsrc->configReg =  config;
	led1642Shift(pRsrc, pRsrc->configReg, (16-7));		//config reg latch, LE keep high for last 7 sck
}

static void getConfig	(LED1642_Rsrc_T* pRsrc, u16* config){
	led1642Shift(pRsrc, 0, (16-8));		//config reg latch, LE keep high for last 7 sck
	*config = led1642Read(pRsrc);
}

static void setResolution	(LED1642_Rsrc_T* pRsrc, u8 res){
	if(res==0)	pRsrc->configReg &= 0x7ffff ;		//12 bit
	else	pRsrc->configReg |= 0x8000 ;					//16 bit
	led1642Shift(pRsrc, pRsrc->configReg, (16-7));		//config reg latch, LE keep high for last 7 sck	
}

static void led1642Shift(LED1642_Rsrc_T* pRsrc, u16 dat, u8 leUpIndx){
	u8 i;
	HAL_GPIO_WritePin(pRsrc->LE.GPIOx, pRsrc->LE.GPIO_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_RESET);
	for(i=0;i<16;i++){
//		HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_RESET);
		if(dat & (0x8000>>i))	HAL_GPIO_WritePin(pRsrc->SDI.GPIOx, pRsrc->SDI.GPIO_Pin, GPIO_PIN_SET);
		else	HAL_GPIO_WritePin(pRsrc->SDI.GPIOx, pRsrc->SDI.GPIO_Pin, GPIO_PIN_RESET);
		if(i >= leUpIndx)	HAL_GPIO_WritePin(pRsrc->LE.GPIOx, pRsrc->LE.GPIO_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_SET);	
		HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_RESET);	
	}
	HAL_GPIO_WritePin(pRsrc->LE.GPIOx, pRsrc->LE.GPIO_Pin, GPIO_PIN_RESET);
}

static u16 led1642Read(LED1642_Rsrc_T* pRsrc){
	u8 i;
	u16 dat = 0;
	for(i=0;i<16;i++){
		HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_SET);	
		dat <<= 1;
//		if(HAL_GPIO_ReadPin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin) == GPIO_PIN_SET)	dat |= 0x01;
		HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_RESET);	
	}
}

/**********************************************************
 == THE END ==
**********************************************************/
