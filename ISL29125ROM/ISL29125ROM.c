/**********************************************************
filename: ISL29125.c
**********************************************************/

/************************包含头文件***********************/
#include "board.h"
#include "publicRsrc.h"
#include "ISL29125ROM.h"

#define DEV_RESET 0x46

typedef enum {
	ISL291259_X16  = 0<<4,
	ISL291259_X12  = 1<<4,
} ISL291259_RESOLUTION;

//extern PIN_T TP;
//extern UART_DEV myUart;


#define GAIN_DIV 10000
#define GAIN_RD_RD_MUL 5198
#define GAIN_RD_GN_MUL 3622
#define GAIN_RD_BL_MUL 1179

#define GAIN_GN_RD_MUL 3775
#define GAIN_GN_GN_MUL 5180
#define GAIN_GN_BL_MUL 1076

#define GAIN_BL_RD_MUL 1413
#define GAIN_BL_GN_MUL 1247
#define GAIN_BL_BL_MUL 7340

/**********************************************************
 Private function
**********************************************************/
static void isl29125Reset(ISL291259_Rsrc_T* pRsrc);
static DEV_STATUS isl29125ReadReg(ISL291259_Rsrc_T* pRsrc, u8 addr, u8 *pBuf, u8 nBytes);
static DEV_STATUS isl29125WriteReg(ISL291259_Rsrc_T* pRsrc, u8 addr, u8 *pBuf, u8 nBytes);
static DEV_STATUS isl29125Cfg(ISL291259_Rsrc_T *pRsrc, ISL291259_MODE mode, ISL291259_SENSITIVITY sns, ISL291259_RESOLUTION res);
static DEV_STATUS readAdc(ISL291259_Rsrc_T* pRsrc);

static DEV_STATUS startConvert(ISL291259_Rsrc_T *pRsrc, RGB_SENSOR_MODE mode, u16 times);
static DEV_STATUS stopConvert(ISL291259_Rsrc_T *pRsrc);

static DEV_STATUS isl29125GainCal(ISL291259_Rsrc_T *pRsrc, RGB_SENSOR_MODE mode, u16 trueVal);
static DEV_STATUS isl29125OffsetCal(ISL291259_Rsrc_T *pRsrc, RGB_SENSOR_MODE mode);
static DEV_STATUS isl29125ResetCal(ISL291259_Rsrc_T *pRsrc, RGB_SENSOR_MODE mode, CAL32_T cal);
//void ISL291259_Setup(ISL291259_Dev_T *pDev, const u8 *NAME, PIN_T scl, PIN_T sda, PIN_T irq);
//ISL291259_Dev_T *rgbSensorFine(const u8* NAME);
static void isl29125ResetCallBackMsg(ISL291259_Rsrc_T* pRsrc);
static void isl29125SendCallBackMsg(ISL291259_Rsrc_T* pRsrc);

//apply function
static s8 applyCross	(ISL291259_Rsrc_T* pRsrc, u16* val);
static s8 applyCal		(ISL291259_Rsrc_T* pRsrc, u16* val);
static s8 average			(ISL291259_Rsrc_T* pRsrc, u16* val);

//ROM OP
static void ht2201ReadCfg(ISL291259_Rsrc_T* pRsrc, RGB_SENSOR_MODE mode);
static void ht2201WriteCfg(ISL291259_Rsrc_T* pRsrc, RGB_SENSOR_MODE mode);
static void checkCfg(ISL291259_Rsrc_T* pRsrc, CAL32_T* pCal, RGB_SENSOR_MODE mode);

//extern UART_DEV myUart;
/**********************************************************
 Public function
**********************************************************/
s8 isl29125DevSetup(
	ISL291259_Dev_T *pDev, 
	const DEV_PORT* pPort,
	void (*SendNewMsg)	(const char* MSG) 
){
	u8 tmpU8;//recName[32];
	ISL291259_Rsrc_T *pRsrc = &pDev->rsrc;
	
	PIN_OP.As_OUTPUT_OD_PULLUP_HIGH(pPort->PA0);
	PIN_OP.As_OUTPUT_OD_PULLUP_HIGH(pPort->PA1);
	PIN_OP.As_OUTPUT_OD_PULLUP_HIGH(pPort->PA2);	

	//resource
	pRsrc->INT = pPort->PA0;
	IIC_IO_Setup(&pRsrc->DevIIC, pPort->PA1, pPort->PA2, miscDelayMs256);
	ht2201DevSetup(&pRsrc->ht2201, pPort->PA1, pPort->PA2);
	//reset
	isl29125Reset(pRsrc);	miscDelay(200);
	pRsrc->msgEn = 0;
	//get ID
	pRsrc->ID = 0;
	if(isl29125ReadReg(pRsrc, 0x00, &tmpU8, 1) == DEV_SUCCESS)	pRsrc->ID = tmpU8;
	miscDelay(200);
	//pull low INT pin per conversion
	isl29125WriteReg(pRsrc,0x03, &tmpU8, 1);
	//REG delegate
	pRsrc->SendNewMsg = SendNewMsg;
	//ops
	pDev->Reset = isl29125Reset;
	pDev->Polling = readAdc;
	pDev->StartConvert = startConvert;
	pDev->StopConvert = stopConvert;
	pDev->OffsetCal = isl29125OffsetCal;
	pDev->GainCal = isl29125GainCal;
	pDev->ResetCal = isl29125ResetCal;
	pDev->ResetCallBackMsg = isl29125ResetCallBackMsg;
	//read cal
	pRsrc->calCfg.RL.offset = 0;
	pRsrc->calCfg.RL.gainMul = 1;
	pRsrc->calCfg.RL.gainDiv = 1;
	pRsrc->calCfg.GL = pRsrc->calCfg.RL;
	pRsrc->calCfg.BL = pRsrc->calCfg.RL;
	pRsrc->calCfg.RH = pRsrc->calCfg.RL;
	pRsrc->calCfg.GH = pRsrc->calCfg.RL;
	pRsrc->calCfg.BH = pRsrc->calCfg.RL;

	//READ FROM EEPROM
	ht2201ReadCfg(pRsrc, R_LOW_RNG);
	checkCfg(pRsrc, &pRsrc->calCfg.RL, R_LOW_RNG);
	ht2201ReadCfg(pRsrc, G_LOW_RNG);
	checkCfg(pRsrc, &pRsrc->calCfg.GL, G_LOW_RNG);
	ht2201ReadCfg(pRsrc, B_LOW_RNG);
	checkCfg(pRsrc, &pRsrc->calCfg.BL, B_LOW_RNG);
	ht2201ReadCfg(pRsrc, R_HIGH_RNG);
	checkCfg(pRsrc, &pRsrc->calCfg.RH, R_HIGH_RNG);
	ht2201ReadCfg(pRsrc, G_HIGH_RNG);
	checkCfg(pRsrc, &pRsrc->calCfg.GH, G_HIGH_RNG);
	ht2201ReadCfg(pRsrc, B_HIGH_RNG);
	checkCfg(pRsrc, &pRsrc->calCfg.BH, B_HIGH_RNG);
	return 0;
}
		
static void checkCfg(ISL291259_Rsrc_T* pRsrc, CAL32_T* pCal, RGB_SENSOR_MODE mode){
	if(pCal->gainDiv == 0 || pCal->gainMul == 0){
		pCal->gainDiv = 1;
		pCal->gainMul = 1;
		pCal->offset = 0;
		ht2201WriteCfg(pRsrc, mode);
	}
}
/*******************************************************************************
* Function Name  : device fine
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//ISL291259_Dev_T *rgbSensorFine(const u8* NAME){
//	u8 i,j,k,len0,len1;
//	
//	len0 = strlen((const char*)NAME);
//	if(len0 <=0 )	return NULL;
//	//try same name device first
//	for(i=0;i<RGB_SENSOR_CNT; i++){
//		len1 = strlen((const char*)DevRgbSensors[i].rsrc.name);
//		if(len0 != len1)	continue;
//		for(j=0,k=0;j<len0;j++){
//			if(NAME[j] != DevRgbSensors[i].rsrc.name[i])	{k++;	break;}
//		}
//		if(k==0)	return &DevRgbSensors[i];
//	}
//	return NULL;
//}

/**********************************************************
 reset
**********************************************************/
static void isl29125Reset(ISL291259_Rsrc_T* pRsrc){
	u8 tmpU8 = DEV_RESET;
	isl29125WriteReg(pRsrc, 0x00, &tmpU8, 1);
}
/**********************************************************
 set mode and start
**********************************************************/
static DEV_STATUS isl29125Cfg(ISL291259_Rsrc_T *pRsrc, ISL291259_MODE mode, ISL291259_SENSITIVITY sns, ISL291259_RESOLUTION res){
	u8 tmpU8 = mode|sns|res;
	return isl29125WriteReg(pRsrc, 0x01, &tmpU8, 1);
}

/**********************************************************
 read adc (read adc after INT pin pull down)
**********************************************************/
static DEV_STATUS readAdc(ISL291259_Rsrc_T* pRsrc){
	u8 i,j,tmpU8[7];
//	u8 msg[CALLBACK_MSG_LEN] = {0}, numStr[10] = {0};
	u16 *pBuf=NULL, dat;
	u16 val[3];
	
	if(HAL_GPIO_ReadPin(pRsrc->INT.GPIOx, pRsrc->INT.GPIO_Pin) == GPIO_PIN_RESET)
	{
		isl29125ReadReg(pRsrc, 0x08, tmpU8, 7);
		pRsrc->cvrtIndx ++;
		if(tmpU8[0] & 1<< 1){			//if conversion completed
			i = tmpU8[0] >> 4;	i &= 0x03;
			if(i == 1){				//green
				pBuf = pRsrc->adcG;
				dat = tmpU8[2];
				dat <<= 8;	dat |= tmpU8[1];
				for(j=ADC_BUF_LEN-1; j>0; j--)	pBuf[j] = pBuf[j-1];
				pBuf[0] = dat;
			}
			else if(i == 2){	//red 				
				pBuf = pRsrc->adcR;
				dat = tmpU8[4];
				dat <<= 8;	dat |= tmpU8[3];
				for(j=ADC_BUF_LEN-1; j>0; j--)	pBuf[j] = pBuf[j-1];	
				pBuf[0] = dat;	
			}
			else if(i == 3){	//blue
				pBuf = pRsrc->adcB;
				dat = tmpU8[6];
				dat <<= 8;	dat |= tmpU8[5];
				for(j=ADC_BUF_LEN-1; j>0; j--)	pBuf[j] = pBuf[j-1];
				pBuf[0] = dat;
			}
			else {}
			
			average(pRsrc, val);
			if(pRsrc->outMethod == 1){	//apply cal
				applyCal(pRsrc, val);
				pRsrc->outGn = val[0];
				pRsrc->outRd = val[1];
				pRsrc->outBl = val[2];
			}
			else if(pRsrc->outMethod == 2){		//apply cross
				applyCross(pRsrc, val);
				pRsrc->outGn = val[0];
				pRsrc->outRd = val[1];
				pRsrc->outBl = val[2];
			}
			else if(pRsrc->outMethod == 3){		//apply cal + gain
				applyCal(pRsrc, val);
				applyCross(pRsrc, val);
				pRsrc->outGn = val[0];
				pRsrc->outRd = val[1];
				pRsrc->outBl = val[2];
			}
			else{
				pRsrc->outGn = pRsrc->adcG[0];
				pRsrc->outRd = pRsrc->adcR[0];
				pRsrc->outBl = pRsrc->adcB[0];
			}
			if(pRsrc->cvrtIndx > 4){
				isl29125SendCallBackMsg(pRsrc);
				if(pRsrc->cvrtTimes != 1){
					if(pRsrc->cvrtTimes>1)	pRsrc->cvrtTimes--;
				}
				else{
					pRsrc->cvrtTimes = 0;
					if( stopConvert(pRsrc)	== DEV_ERROR)	return DEV_ERROR;	//stop convertion	
				}		
			}
		}
	}
	return DEV_SUCCESS;
}

static s8 average(ISL291259_Rsrc_T* pRsrc, u16* val){
	u32 tmpU32;
	u8 i;
	//gn
	val[0] = 0;
	for(i=0,tmpU32 = 0; i<ADC_BUF_LEN; i++){
		if(pRsrc->adcG[i] == 0)	break;
		tmpU32 += pRsrc->adcG[i];
	}
	if(i){
		tmpU32 /= i;
		if((tmpU32 & 0xffff0000)	== 0 )	val[0] = tmpU32;
	}
	//rd
	val[1] = 0;
	for(i=0,tmpU32 = 0; i<ADC_BUF_LEN; i++){
		if(pRsrc->adcR[i] == 0)	break;
		tmpU32 += pRsrc->adcR[i];
	}
	if(i){
		tmpU32 /= i;
		if((tmpU32 & 0xffff0000 )	== 0 )	val[1] = tmpU32;
	}
	//bl
	val[2] = 0;
	for(i=0,tmpU32 = 0; i<ADC_BUF_LEN; i++){
		if(pRsrc->adcB[i] == 0)	break;
		tmpU32 += pRsrc->adcB[i];
	}
	if(i){
		tmpU32 /= i;
		if((tmpU32 & 0xffff0000)	== 0 )	val[2] = tmpU32;
	}
	return 0;
}

static s8 applyCal(ISL291259_Rsrc_T* pRsrc, u16* val){
	u16 dat;
	u32 tmpU32;
	CAL32_T *pCal;

	//green
	dat = val[0];
	pCal = &pRsrc->calCfg.GL;
	if(pRsrc->curMode >= R_HIGH_RNG)	pCal = &pRsrc->calCfg.GH;
	tmpU32 = (dat>pCal->offset ? (dat-pCal->offset):0);
	tmpU32 *= pCal->gainMul;	tmpU32 /= pCal->gainDiv;
	val[0] = tmpU32;
	//red
	dat = val[1];
	pCal = &pRsrc->calCfg.RL;
	if(pRsrc->curMode >= R_HIGH_RNG)	pCal = &pRsrc->calCfg.RH;
	tmpU32 = dat;
	if(tmpU32 > pCal->offset){
		tmpU32 -= pCal->offset;
		tmpU32 *= pCal->gainMul;	
		tmpU32 /= pCal->gainDiv;
	}
	val[1] = tmpU32;
	//blue
	dat = val[2];
	pCal = &pRsrc->calCfg.BL;
	if(pRsrc->curMode >= R_HIGH_RNG)	pCal = &pRsrc->calCfg.BH;
	tmpU32 = (dat>pCal->offset ? (dat-pCal->offset):0);
	tmpU32 *= pCal->gainMul;	tmpU32 /= pCal->gainDiv;	
	val[2] = tmpU32;
	
	return 0;
}

static s8 applyCross(ISL291259_Rsrc_T* pRsrc, u16* val){
	u16 valX[3];
	u32 tmp[3];

	//green
	tmp[0] = val[0];	tmp[0] *= GAIN_GN_GN_MUL;	tmp[0] /= GAIN_DIV;
	tmp[1] = val[1];	tmp[1] *= GAIN_GN_RD_MUL;	tmp[1] /= GAIN_DIV;
	tmp[2] = val[2];	tmp[2] *= GAIN_GN_BL_MUL;	tmp[2] /= GAIN_DIV;
	tmp[0] += (tmp[1] + tmp[2]);
	if((tmp[0]) & 0xffff0000)	return -1;
	valX[0] = tmp[0];
	//red
	tmp[0] = val[0];	tmp[0] *= GAIN_RD_GN_MUL;	tmp[0] /= GAIN_DIV;
	tmp[1] = val[1];	tmp[1] *= GAIN_RD_RD_MUL;	tmp[1] /= GAIN_DIV;
	tmp[2] = val[2];	tmp[2] *= GAIN_RD_BL_MUL;	tmp[2] /= GAIN_DIV;
	tmp[0] += (tmp[1] + tmp[2]);	
	if((tmp[0]) & 0xffff0000)	return -1;
	valX[1] = tmp[0];
	//blue
	tmp[0] = val[0];	tmp[0] *= GAIN_BL_GN_MUL;	tmp[0] /= GAIN_DIV;
	tmp[1] = val[1];	tmp[1] *= GAIN_BL_RD_MUL;	tmp[1] /= GAIN_DIV;
	tmp[2] = val[2];	tmp[2] *= GAIN_BL_BL_MUL;	tmp[2] /= GAIN_DIV;
	tmp[0] += (tmp[1] + tmp[2]);	
	if((tmp[0]) & 0xffff0000)	return -1;
	valX[2] = tmp[0];
	//copy to
	val[0] = valX[0];
	val[1] = valX[1];
	val[2] = valX[2];
	return 0;
}

/**********************************************************
 read data
**********************************************************/
static DEV_STATUS isl29125ReadReg(ISL291259_Rsrc_T* pRsrc, u8 addr, u8 *pBuf, u8 nBytes){
	u8 i;

	IIC_IO_Dev_T *pI2cDev = &pRsrc->DevIIC;
	IIC_IO_Rsrc_T *pI2cRsrc = &pI2cDev->rsrc;

	//make start
	pI2cDev->Start(pI2cRsrc);
	//device addr
	pI2cDev->WriteByte(pI2cRsrc,0x88);
	if(pI2cDev->WaitAck(pI2cRsrc) == 1)	{
		pI2cDev->Stop(pI2cRsrc);
		return DEV_ERROR;
	}
	//word addr
	pI2cDev->WriteByte(pI2cRsrc,addr);
	if(pI2cDev->WaitAck(pI2cRsrc) == 1)	{
		pI2cDev->Stop(pI2cRsrc);
		return DEV_ERROR;
	}
	//re-start
	pI2cDev->Restart(pI2cRsrc);
	//device addr
	pI2cDev->WriteByte(pI2cRsrc, 0x89);
	if(pI2cDev->WaitAck(pI2cRsrc) == 1)	{
		pI2cDev->Stop(pI2cRsrc);
		return DEV_ERROR;
	}
	//read data
	for(i=0;i<nBytes;i++){
		pBuf[i] = pI2cDev->ReadByte(pI2cRsrc);
		if(i < nBytes-1)
			pI2cDev->WriteAck(pI2cRsrc);
	}
	//stop
	pI2cDev->Stop(pI2cRsrc);
	miscDelay(250);
	return DEV_SUCCESS;
}

/**********************************************************
 write data
**********************************************************/
static DEV_STATUS isl29125WriteReg(ISL291259_Rsrc_T* pRsrc, u8 addr, u8 *pBuf, u8 nBytes){
	u8 i;
	IIC_IO_Dev_T *pI2cDev = &pRsrc->DevIIC;
	IIC_IO_Rsrc_T *pI2cRsrc = &pI2cDev->rsrc;
	
	for(i=0;i<nBytes;i++){
		//make start
		pI2cDev->Start(pI2cRsrc);
		//device addr
		pI2cDev->WriteByte(pI2cRsrc,0x88);
		if(pI2cDev->WaitAck(pI2cRsrc) == 1)	{
			pI2cDev->Stop(pI2cRsrc);
			return DEV_ERROR;
		}
		//word addr
		pI2cDev->WriteByte(pI2cRsrc,addr);
		if(pI2cDev->WaitAck(pI2cRsrc) == 1)	{
			pI2cDev->Stop(pI2cRsrc);
			return DEV_ERROR;
		}
		//write data
		pI2cDev->WriteByte(pI2cRsrc,pBuf[i]);
		if(pI2cDev->WaitAck(pI2cRsrc) == 1)	{
			pI2cDev->Stop(pI2cRsrc);
			return DEV_ERROR;
		}
		//stop
		pI2cDev->Stop(pI2cRsrc);
		//delay for Twc
		miscDelay(250);
		miscDelay(250);
		miscDelay(250);
		miscDelay(250);
		miscDelay(250);
		miscDelay(250);

	}
	return DEV_SUCCESS;
}

/**********************************************************
	startConvert
**********************************************************/
static DEV_STATUS startConvert(ISL291259_Rsrc_T *pRsrc, RGB_SENSOR_MODE mode, u16 times){
	u8 i;
	ISL291259_MODE islMode;
	ISL291259_SENSITIVITY sns;
	
	if			(mode == R_LOW_RNG)	{islMode = ISL291259_R;	sns = ISL291259_LOW;}
	else if	(mode == G_LOW_RNG)	{islMode = ISL291259_G;	sns = ISL291259_LOW;}
	else if	(mode == B_LOW_RNG)	{islMode = ISL291259_B;	sns = ISL291259_LOW;}
	else if	(mode == RGB_LOW_RNG)	{islMode = ISL291259_RGB;	sns = ISL291259_LOW;}
	else if	(mode == R_HIGH_RNG)	{islMode = ISL291259_R;	sns = ISL291259_HIGH;}
	else if	(mode == G_HIGH_RNG)	{islMode = ISL291259_G;	sns = ISL291259_HIGH;}
	else if	(mode == B_HIGH_RNG)	{islMode = ISL291259_B;	sns = ISL291259_HIGH;}
	else if	(mode == RGB_HIGH_RNG)	{islMode = ISL291259_RGB;	sns = ISL291259_HIGH;}
	else{}

	//前3个样品丢弃

	if(isl29125Cfg(pRsrc, islMode, sns, ISL291259_X16) == DEV_ERROR)	return DEV_ERROR;
	isl29125ReadReg(pRsrc, 0x08, &i, 1);
		
//	for(i=0;i<2;){
//		if(HAL_GPIO_ReadPin(pRsrc->INT.GPIOx, pRsrc->INT.GPIO_Pin) == GPIO_PIN_RESET){
//			readReg(pRsrc, 0x08, &i, 1);
//			i++;
//		}
//		osDelay(32);
//	}

	//正式启动采样
	pRsrc->msgEn = 1;
	pRsrc->cvrtTimes = times;
//	if(isl29125Cfg(pRsrc, islMode, sns, ISL291259_X16) == DEV_ERROR)	return DEV_ERROR;
	for(i=0;i<ADC_BUF_LEN;i++){
		pRsrc->adcR[i] = 0;
		pRsrc->adcG[i] = 0;
		pRsrc->adcB[i] = 0;
	}
	pRsrc->curMode = mode;
	pRsrc->cvrtIndx = 0;
	return DEV_SUCCESS;
}

/**********************************************************
	stopConvert
**********************************************************/
static DEV_STATUS stopConvert(ISL291259_Rsrc_T *pRsrc){
	u8 tmpU8 = ISL291259_STBY;
	pRsrc->msgEn = 0;
	return isl29125WriteReg(pRsrc, 0x01, &tmpU8, 1);
}

/**********************************************************
	isl29125GainCal
**********************************************************/
static CAL32_T  *runCalConvert(ISL291259_Rsrc_T* pRsrc, RGB_SENSOR_MODE mode, u16 *result){
	u8 i,msgEn;	u16 *pDat;
	u32 sum = 0;
	CAL32_T *pCal;

	ISL291259_MODE islMode = ISL291259_RGB;
	ISL291259_SENSITIVITY sns = ISL291259_HIGH;
	
	if(mode == R_LOW_RNG)	{
		islMode = ISL291259_R;	
		sns = ISL291259_LOW;
		pDat = pRsrc->adcR;
		pCal = &pRsrc->calCfg.RL;
	}
	else if(mode == G_LOW_RNG)	{
		islMode = ISL291259_G;	
		sns = ISL291259_LOW;	
		pDat = pRsrc->adcG;
		pCal = &pRsrc->calCfg.GL;
	}
	else if(mode == B_LOW_RNG)	{
		islMode = ISL291259_B;	
		sns = ISL291259_LOW;
		pDat = pRsrc->adcB;
		pCal = &pRsrc->calCfg.BL;
	}
	else if(mode == R_HIGH_RNG)	{
		islMode = ISL291259_R;
		sns = ISL291259_HIGH;
		pDat = pRsrc->adcR;
		pCal = &pRsrc->calCfg.RH;
	}
	else if(mode == G_HIGH_RNG)	{
		islMode = ISL291259_G;	
		sns = ISL291259_HIGH;
		pDat = pRsrc->adcG;
		pCal = &pRsrc->calCfg.GH;
	}
	else if(mode == B_HIGH_RNG)	{
		islMode = ISL291259_B;	
		sns = ISL291259_HIGH;
		pDat = pRsrc->adcB;
		pCal = &pRsrc->calCfg.BH;
	}
	else{}

	//转换16次
	pRsrc->cvrtTimes = ADC_BUF_LEN;	//切换现场
	msgEn = pRsrc->msgEn;
	pRsrc->msgEn = 0;
	if(isl29125Cfg(pRsrc, islMode, sns, ISL291259_X16) == DEV_ERROR)	return NULL;
	//等待转换完成
	while(pRsrc->cvrtTimes){	readAdc(pRsrc);	}
	pRsrc->msgEn = msgEn;			//恢复现场
	for (i=0,sum=0;i<ADC_BUF_LEN;i++)	sum += pDat[i];	//求平均值
	sum /= ADC_BUF_LEN;
	*result = sum;
	return pCal;
}

static DEV_STATUS isl29125GainCal(ISL291259_Rsrc_T *pRsrc, RGB_SENSOR_MODE mode, u16 trueVal){
	u16 result;	CAL32_T *pCal = NULL;
	char recName[32];

	strCpy(recName, 32, pRsrc->name);	
	switch(mode){
		case R_LOW_RNG:		strcat((char*)recName, ".cal.rl");	break;
		case G_LOW_RNG:		strcat((char*)recName, ".cal.gl");	break;
		case B_LOW_RNG:		strcat((char*)recName, ".cal.bl");	break;
		case R_HIGH_RNG:	strcat((char*)recName, ".cal.rh");	break;
		case G_HIGH_RNG:	strcat((char*)recName, ".cal.gh");	break;
		case B_HIGH_RNG:	strcat((char*)recName, ".cal.bh");	break;
		default:	return DEV_ERROR;
	}
	//do cal
	pCal = runCalConvert(pRsrc, mode, &result);
	if( pCal == NULL)	return DEV_ERROR;
	//保存校准结果
	if(result == 0)	result = 1;
	pCal->gainDiv = result;
	pCal->gainMul = trueVal;
	//写入文件
//	pRsrc->WriteCalCfg(recName, *pCal);
	ht2201WriteCfg(pRsrc, mode);

	return DEV_SUCCESS;
}

static DEV_STATUS isl29125OffsetCal(ISL291259_Rsrc_T *pRsrc, RGB_SENSOR_MODE mode){
	u16 result;	CAL32_T *pCal = NULL;
	char recName[32];

	strCpy(recName, 32, pRsrc->name);	
	switch(mode){
		case R_LOW_RNG:		strcat((char*)recName, ".cal.rl");	break;
		case G_LOW_RNG:		strcat((char*)recName, ".cal.gl");	break;
		case B_LOW_RNG:		strcat((char*)recName, ".cal.bl");	break;
		case R_HIGH_RNG:	strcat((char*)recName, ".cal.rh");	break;
		case G_HIGH_RNG:	strcat((char*)recName, ".cal.gh");	break;
		case B_HIGH_RNG:	strcat((char*)recName, ".cal.bh");	break;
		default:	return DEV_ERROR;
	}
	//do calibration
	pCal = runCalConvert(pRsrc, mode, &result);
	if( pCal == NULL)	return DEV_ERROR;
	//保存校准结果
	if(result>0xff)	result = 0xff;
	pCal->offset = 12;
	//写入文件
//	pRsrc->WriteCalCfg(recName, *pCal);
	ht2201WriteCfg(pRsrc, mode);
	return DEV_SUCCESS;
}

static DEV_STATUS isl29125ResetCal(ISL291259_Rsrc_T *pRsrc, RGB_SENSOR_MODE mode, CAL32_T cal){
	char recName[32];

	strCpy(recName, 32, pRsrc->name);	
	switch(mode){
		case R_LOW_RNG:	
			pRsrc->calCfg.RL.offset = cal.offset;
			pRsrc->calCfg.RL.gainMul = cal.gainMul;
			pRsrc->calCfg.RL.gainDiv = cal.gainDiv;
			strCpy(recName, 32, pRsrc->name);
			strcat((char*)recName, ".cal.rl");	
//			pRsrc->WriteCalCfg(recName, pRsrc->calCfg.RL);
			ht2201WriteCfg(pRsrc, R_LOW_RNG);
			return DEV_SUCCESS;
		case G_LOW_RNG:		
			pRsrc->calCfg.GL.offset = cal.offset;
			pRsrc->calCfg.GL.gainMul = cal.gainMul;
			pRsrc->calCfg.GL.gainDiv = cal.gainDiv;
			strCpy(recName, 32, pRsrc->name);
			strcat((char*)recName, ".cal.gl");	
//			pRsrc->WriteCalCfg(recName, pRsrc->calCfg.GL);
			ht2201WriteCfg(pRsrc, G_LOW_RNG);	
			return DEV_SUCCESS;
		case B_LOW_RNG:		
			pRsrc->calCfg.BL.offset = cal.offset;
			pRsrc->calCfg.BL.gainMul = cal.gainMul;
			pRsrc->calCfg.BL.gainDiv = cal.gainDiv;
			strCpy(recName, 32, pRsrc->name);
			strcat((char*)recName, ".cal.bl");	
//			pRsrc->WriteCalCfg(recName, pRsrc->calCfg.BL);
			ht2201WriteCfg(pRsrc, B_LOW_RNG);	
			return DEV_SUCCESS;
		case R_HIGH_RNG:	
			pRsrc->calCfg.RH.offset = cal.offset;
			pRsrc->calCfg.RH.gainMul = cal.gainMul;
			pRsrc->calCfg.RH.gainDiv = cal.gainDiv;
			strCpy(recName, 32, pRsrc->name);
			strcat((char*)recName, ".cal.rh");	
//			pRsrc->WriteCalCfg(recName, pRsrc->calCfg.RH);
			ht2201WriteCfg(pRsrc, R_HIGH_RNG);
			return DEV_SUCCESS;
		case G_HIGH_RNG:	
			pRsrc->calCfg.GH.offset = cal.offset;
			pRsrc->calCfg.GH.gainMul = cal.gainMul;
			pRsrc->calCfg.GH.gainDiv = cal.gainDiv;
			strCpy(recName, 32, pRsrc->name);
			strcat((char*)recName, ".cal.gh");	
//			pRsrc->WriteCalCfg(recName, pRsrc->calCfg.GH);
			ht2201WriteCfg(pRsrc, G_HIGH_RNG);
			return DEV_SUCCESS;
		case B_HIGH_RNG:	
			pRsrc->calCfg.BH.offset = cal.offset;
			pRsrc->calCfg.BH.gainMul = cal.gainMul;
			pRsrc->calCfg.BH.gainDiv = cal.gainDiv;
			strCpy(recName, 32, pRsrc->name);
			strcat((char*)recName, ".cal.bh");	
//			pRsrc->WriteCalCfg(recName, pRsrc->calCfg.BH);
			ht2201WriteCfg(pRsrc, B_HIGH_RNG);
			return DEV_SUCCESS;
		case RGB_LOW_RNG:
			pRsrc->calCfg.RL.offset = cal.offset;
			pRsrc->calCfg.RL.gainMul = cal.gainMul;
			pRsrc->calCfg.RL.gainDiv = cal.gainDiv;
			pRsrc->calCfg.GL.offset = cal.offset;
			pRsrc->calCfg.GL.gainMul = cal.gainMul;
			pRsrc->calCfg.GL.gainDiv = cal.gainDiv;
			pRsrc->calCfg.BL.offset = cal.offset;
			pRsrc->calCfg.BL.gainMul = cal.gainMul;
			pRsrc->calCfg.BL.gainDiv = cal.gainDiv;
			strCpy(recName, 32, pRsrc->name);
			strcat((char*)recName, ".cal.rl");	
//			pRsrc->WriteCalCfg(recName, pRsrc->calCfg.RL);
			ht2201WriteCfg(pRsrc, R_LOW_RNG);
			strCpy(recName, 32, pRsrc->name);
			strcat((char*)recName, ".cal.gl");	
//			pRsrc->WriteCalCfg(recName, pRsrc->calCfg.GL);
			ht2201WriteCfg(pRsrc, G_LOW_RNG);
			strCpy(recName, 32, pRsrc->name);
			strcat((char*)recName, ".cal.bl");	
//			pRsrc->WriteCalCfg(recName, pRsrc->calCfg.BL);
			ht2201WriteCfg(pRsrc, B_LOW_RNG);
			return DEV_SUCCESS;
		case RGB_HIGH_RNG:
			pRsrc->calCfg.RH.offset = cal.offset;
			pRsrc->calCfg.RH.gainMul = cal.gainMul;
			pRsrc->calCfg.RH.gainDiv = cal.gainDiv;
			pRsrc->calCfg.GH.offset = cal.offset;
			pRsrc->calCfg.GH.gainMul = cal.gainMul;
			pRsrc->calCfg.GH.gainDiv = cal.gainDiv;
			pRsrc->calCfg.BH.offset = cal.offset;
			pRsrc->calCfg.BH.gainMul = cal.gainMul;
			pRsrc->calCfg.BH.gainDiv = cal.gainDiv;
			strCpy(recName, 32, pRsrc->name);
			strcat((char*)recName, ".cal.rh");	
//			pRsrc->WriteCalCfg(recName, pRsrc->calCfg.RH);
			ht2201WriteCfg(pRsrc, R_HIGH_RNG);
			strCpy(recName, 32, pRsrc->name);
			strcat((char*)recName, ".cal.gh");	
//			pRsrc->WriteCalCfg(recName, pRsrc->calCfg.GH);
			ht2201WriteCfg(pRsrc, G_HIGH_RNG);
			strCpy(recName, 32, pRsrc->name);
			strcat((char*)recName, ".cal.bh");	
//			pRsrc->WriteCalCfg(recName, pRsrc->calCfg.BH);
			ht2201WriteCfg(pRsrc, B_HIGH_RNG);
			return DEV_SUCCESS;
		default:	return DEV_ERROR;
	}
}

static void isl29125ResetCallBackMsg(ISL291259_Rsrc_T* pRsrc){
	u8 i;
	for(i=0;i<CALLBACK_MSG_LEN;i++)	
		pRsrc->callBackMsg[i] = 0;
}

static void isl29125SendCallBackMsg(ISL291259_Rsrc_T* pRsrc){
	
	if(pRsrc->SendNewMsg == NULL)	return;
	if(pRsrc->msgEn){
		memset(pRsrc->callBackMsg, 0, CALLBACK_MSG_LEN);
		strCpy(pRsrc->callBackMsg, CALLBACK_MSG_LEN, "+msg@");
		strCatX(pRsrc->callBackMsg, pRsrc->name, ".adc(", NULL);
		CONVERT.Uint32ToStrCatDecN(pRsrc->callBackMsg, pRsrc->outRd, 5);
		strcat((char*)pRsrc->callBackMsg, ",");
		CONVERT.Uint32ToStrCatDecN(pRsrc->callBackMsg, pRsrc->outGn, 5);
		strcat((char*)pRsrc->callBackMsg, ",");
		CONVERT.Uint32ToStrCatDecN(pRsrc->callBackMsg, pRsrc->outBl, 5);
//		//sum
//		strcat((char*)pRsrc->callBackMsg, ",");
//		sum = pRsrc->adcR[0] + pRsrc->adcG[0] + pRsrc->adcB[0];
//		CONVERT.Uint32ToStrCatDecN(pRsrc->callBackMsg, sum, 8);
		strCatX(pRsrc->callBackMsg, ")", NULL, NULL);
		if(pRsrc->SendNewMsg != NULL)	pRsrc->SendNewMsg(pRsrc->callBackMsg);
	}
}

static void ht2201ReadCfg(ISL291259_Rsrc_T* pRsrc, RGB_SENSOR_MODE mode){
	u8 tmp,addr,pBuf[10] = {0xff};
	CAL32_T *pCal;

	tmp = mode;
	switch(tmp){
		case R_LOW_RNG:	addr = 2;				pCal = &pRsrc->calCfg.RL;	break;
		case G_LOW_RNG:	addr = 10+2;		pCal = &pRsrc->calCfg.GL;	break;
		case B_LOW_RNG:	addr = 20+2;		pCal = &pRsrc->calCfg.BL;	break;
		case R_HIGH_RNG:	addr = 30+2;	pCal = &pRsrc->calCfg.RH;	break;
		case G_HIGH_RNG:	addr = 40+2;	pCal = &pRsrc->calCfg.GH;	break;
		case B_HIGH_RNG:	addr = 50+2;	pCal = &pRsrc->calCfg.BH;	break;
		default:	
			return;
	}
	pRsrc->ht2201.Read(&pRsrc->ht2201.rsrc,addr,pBuf,10);
	//load to calCfg
	pCal->offset = CONVERT.U8ArrayToUint16(&pBuf[0]);
	pCal->gainMul = CONVERT.U8ArrayToUint32(&pBuf[2]);
	pCal->gainDiv = CONVERT.U8ArrayToUint32(&pBuf[6]);

	return ;
}

static void ht2201WriteCfg(ISL291259_Rsrc_T* pRsrc, RGB_SENSOR_MODE mode){
	u8 tmp,addr,pBuf[10];
	CAL32_T *pCal;

	tmp = mode;
	switch(tmp){
		case R_LOW_RNG:	addr = 2;				pCal = &pRsrc->calCfg.RL;	break;
		case G_LOW_RNG:	addr = 10+2;		pCal = &pRsrc->calCfg.GL;	break;
		case B_LOW_RNG:	addr = 20+2;		pCal = &pRsrc->calCfg.BL;	break;
		case R_HIGH_RNG:	addr = 30+2;	pCal = &pRsrc->calCfg.RH;	break;
		case G_HIGH_RNG:	addr = 40+2;	pCal = &pRsrc->calCfg.GH;	break;
		case B_HIGH_RNG:	addr = 50+2;	pCal = &pRsrc->calCfg.BH;	break;
		default:	
			return;
	}
	//make pBuf
	CONVERT.Uint16ToU8Array(pCal->offset, &pBuf[0]);
	CONVERT.Uint32ToU8Array(pCal->gainMul, &pBuf[2]);
	CONVERT.Uint32ToU8Array(pCal->gainDiv, &pBuf[6]);
	//save in ht2201
	pRsrc->ht2201.Write(&pRsrc->ht2201.rsrc, addr, pBuf, 10);
	return;
}

//static DEV_STATUS readCalx1	(u8 *beginWith, CAL_T *pCal){
//	s8 tmpS8;	PARAM_DEV_T e;
//	u8 calMsg[200] = {0}, doReset = 0;
//	
//	setupParam(&e);
//	if(FileDev->FetchRecord("board.ini", beginWith, calMsg, 64) != DEV_SUCCESS)
//		doReset = 1;
//	else{
//		tmpS8 = fetchFun(calMsg, beginWith, &e);
//		if(tmpS8==3){
//			pCal->offset = *(u32*)e.rsrc.params[0];
//			pCal->gainMul = *(u32*)e.rsrc.params[1];
//			pCal->gainDiv = *(u32*)e.rsrc.params[2];
//		}
//		else	doReset = 1;
//	}
//	if(doReset){
//		strCpy(calMsg,64,beginWith);
//		strCat(calMsg,64,"(0x00,0x00000001,0x00000001)\r\n");
//		FileDev->WriteRecord("board.ini", beginWith, calMsg);
//		pCal->offset = 0;
//		pCal->gainMul = 1;
//		pCal->gainDiv = 1;
//	}
//	return DEV_SUCCESS;
//}

//static DEV_STATUS isl29125ReadCalCfg	(const u8 *DEV_NAME, RGB_SENSOR_MODE mode, CAL_T *pCal){
//	u8 beginWith[64] = {0};

//	if(mode == R_LOW_RNG){
//		strCpy(beginWith,64,DEV_NAME);	
//		strCat(beginWith,64,".rl");	
//		readCalx1(beginWith, pCal);
//	}
//	else if(mode == G_LOW_RNG){
//		strCpy(beginWith,64,DEV_NAME);	
//		strCat(beginWith,64,".gl");	
//		readCalx1(beginWith, pCal);
//	}
//	else if(mode == B_LOW_RNG){
//		strCpy(beginWith,64,DEV_NAME);	
//		strCat(beginWith,64,".bl");	
//		readCalx1(beginWith, pCal);
//	}
//	else if(mode==R_HIGH_RNG){
//		strCpy(beginWith,64,DEV_NAME);	
//		strCat(beginWith,64,".rh");	
//		readCalx1(beginWith, pCal);
//	}
//	else if(mode==G_HIGH_RNG){
//		strCpy(beginWith,64,DEV_NAME);	
//		strCat(beginWith,64,".gh");	
//		readCalx1(beginWith, pCal);
//	}
//	else if(mode==B_HIGH_RNG){
//		strCpy(beginWith,64,DEV_NAME);	
//		strCat(beginWith,64,".bh");	
//		readCalx1(beginWith, pCal);
//	}
//	else 	return DEV_ERROR;
//	return DEV_SUCCESS;
//}

//static DEV_STATUS writeCalx1(const u8 *beginWith, const CAL_T cal){
//	u8 calMsg[200] = {0};
//	strCpy(calMsg,200,beginWith);
//	strCat(calMsg,200,"(");
//	num2HexStr8(&calMsg[strlen((const char*)calMsg)], cal.offset);
//	strCat(calMsg,200,",");
//	num2HexStr32(&calMsg[strlen((const char*)calMsg)], cal.gainMul);
//	strCat(calMsg,200,",");
//	num2HexStr32(&calMsg[strlen((const char*)calMsg)], cal.gainDiv);
//	strCat(calMsg,200,")\r\n");
//	FileDev->WriteRecord("board.ini", beginWith, calMsg);
//	return DEV_SUCCESS;
//}

//static DEV_STATUS isl29125WriteCalCfg(const u8* DEV_NAME, RGB_SENSOR_MODE mode, CAL_T cal){
//	u8 beginWith[64] = {0};
//	PARAM_DEV_T e;
//	
//	setupParam(&e);
//	if(mode == R_LOW_RNG){
//		strCpy(beginWith,64,DEV_NAME);	
//		strCat(beginWith,64,".rl");	
//		writeCalx1(beginWith, cal);
//	}
//	else if(mode == G_LOW_RNG){
//		strCpy(beginWith,64,DEV_NAME);	
//		strCat(beginWith,64,".gl");	
//		writeCalx1(beginWith, cal);
//	}
//	else if(mode == B_LOW_RNG){
//		strCpy(beginWith,64,DEV_NAME);	
//		strCat(beginWith,64,".bl");	
//		writeCalx1(beginWith, cal);
//	}
//	else if(mode==R_HIGH_RNG){
//		strCpy(beginWith,64,DEV_NAME);	
//		strCat(beginWith,64,".rh");	
//		writeCalx1(beginWith, cal);
//	}
//	else if(mode==G_HIGH_RNG){
//		strCpy(beginWith,64,DEV_NAME);	
//		strCat(beginWith,64,".gh");	
//		writeCalx1(beginWith, cal);
//	}
//	else if(mode==B_HIGH_RNG){
//		strCpy(beginWith,64,DEV_NAME);	
//		strCat(beginWith,64,".bh");	
//		writeCalx1(beginWith, cal);
//	}
//	else 	return DEV_ERROR;
//	return DEV_SUCCESS;
//}

/**********************************************************
 == THE END ==
**********************************************************/
