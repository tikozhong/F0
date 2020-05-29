/**********************************************************
filename: MCP3421.c
**********************************************************/

/************************包含头文件***********************/
#include "MCP3421.h"
#include "gpioDecal.h"

/**********************************************************
 Private function
**********************************************************/
static DEV_STATUS mcp3421_setMode		(MCP3421_Rsrc_T* pRsrc, MCP3421_Res res, MCP3421_Gain gain);
static DEV_STATUS mcp3421_polling		(MCP3421_Rsrc_T* pRsrc, u8 applyCal);
static DEV_STATUS mcp3421_startConvert	(MCP3421_Rsrc_T *pRsrc, u16 times);
static DEV_STATUS mcp3421_stopConvert	(MCP3421_Rsrc_T *pRsrc);
static DEV_STATUS mcp3421_offsetCal		(MCP3421_Rsrc_T *pRsrc);
static DEV_STATUS mcp3421_gainCal		(MCP3421_Rsrc_T *pRsrc, u32 trueVal);
static DEV_STATUS mcp3421_resetCal		(MCP3421_Rsrc_T *pRsrc, CAL32_T cal);

static void mcp3421_resetCallBackMsg(MCP3421_Rsrc_T* pRsrc);
static void mcp3421_makeCallBackMsg(MCP3421_Rsrc_T* pRsrc, u32 adcVal);

static s8 mcp3421_read(MCP3421_Rsrc_T* pRsrc, u8* rtnBuf);
static s8 mcp3421_write(MCP3421_Rsrc_T* pRsrc, u8* dat, u8 len);

static s8 readADC(MCP3421_Rsrc_T *pRsrc, u32* result);
/**********************************************************
 Public function
**********************************************************/
s8 mcp3421DevSetup(	
		MCP3421_Dev_T *pDev, 
		PIN_T SCL, 
		PIN_T SDA,  
        u8 devAddr,
		void (*SendNewMsg)	(const char* MSG),
		void (*ReadCal)		(const char* NAME, CAL32_T* pCal),
		void (*WriteCal)	(const char* NAME, CAL32_T cal)
){
//	u8 tmpU8;//recName[32];
	memset(pDev,0,sizeof(MCP3421_Dev_T));
	CAL32_T cal;
	MCP3421_Rsrc_T *pRsrc = &pDev->rsrc;
    IIC_IO_Setup(&pRsrc->iic, SCL, SDA);
    pRsrc->devAddr = devAddr;
               
//SendNewMsg((const u8*)"mcp3421DevSetup\r\n");	HAL_Delay(30);
	//register device
//	strCpy(pRsrc->name, DEV_NAME_LEN, NAME);
	//config hardware pin

	pRsrc->Res = SPS004_18BIT;
	pRsrc->Gain = X8;
	pRsrc->msgEn = 0;
	pRsrc->SendNewMsg = SendNewMsg;
	pRsrc->ReadCal = ReadCal;
	pRsrc->WriteCal = WriteCal;

	//ops
	pDev->SetMode = mcp3421_setMode;
	pDev->Polling = mcp3421_polling;
	pDev->StartConvert = mcp3421_startConvert;
	pDev->StopConvert = mcp3421_stopConvert;
	pDev->OffsetCal = mcp3421_offsetCal;
	pDev->GainCal = mcp3421_gainCal;
	pDev->ResetCal = mcp3421_resetCal;
	pDev->ResetCallBackMsg = mcp3421_resetCallBackMsg;
	//READ FROM ROM
    cal.offset = 0;
    cal.gainMul = 1;
    cal.gainDiv = 1;
	
//	pRsrc->ReadCal(pRsrc->name, &cal);
//	if(cal.gainDiv == 0 || cal.gainMul == 0){
//		cal.offset = 0;
//		cal.gainMul = 1;
//		cal.gainDiv = 1;
//		mcp3421_resetCal(pRsrc, cal);
//		SendNewMsg((const u8*)"err cal\r\n");
//	}
	pRsrc->Cal = cal;
//	ht2201ReadCfg(pRsrc, R_LOW_RNG);
//	checkCfg(pRsrc, &pRsrc->calCfg.RL, R_LOW_RNG);
	
	pRsrc->cvrtTimes = 0;
	
	if(mcp3421_setMode(pRsrc, pRsrc->Res, pRsrc->Gain) == DEV_ERROR)
		pRsrc->error |= 0x01;	//mark error at this device
	
	return 0;
}
		
/*********************************************************
* Function Name  : device fine
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*********************************************************/
static DEV_STATUS mcp3421_setMode(MCP3421_Rsrc_T* pRsrc, MCP3421_Res res, MCP3421_Gain gain){
	u8 dat[2] = {0};
	//continue mode, res, gain as param
    dat[0] = 0x90 | ((pRsrc->devAddr&0x07)<<1);
	dat[1] = res&0x03;	dat[1] <<=2;
	dat[1] |= (gain&0x03);
	dat[1] |= 0x10;     //one shoot
	
	if(mcp3421_write(pRsrc, dat, 2) == 0){
		pRsrc->Res = res;
		pRsrc->Gain = gain;
		return DEV_SUCCESS;
	}
	return DEV_ERROR;
}

static DEV_STATUS mcp3421_polling(MCP3421_Rsrc_T* pRsrc, u8 tick){
	u8 i;
	s8 tmpS8;
	u32 result;

	if(pRsrc->msgEn == 0)	return DEV_SUCCESS;

	pRsrc->tick += tick;
	if(pRsrc->tick < 32)    return DEV_SUCCESS;
	pRsrc->tick = 0;
     
	tmpS8 = readADC(pRsrc, &result);
	if(tmpS8 < 0)	return DEV_ERROR;
	else if(tmpS8 == 1){
		for(i=ADC_BUF_LEN;i>1;i--)	pRsrc->result[i-1] = pRsrc->result[i-2];
		pRsrc->result[0] = result;
		mcp3421_makeCallBackMsg(pRsrc, result);	
		if(pRsrc->cvrtTimes == 1){
			if( mcp3421_stopConvert(pRsrc)	== DEV_ERROR)	return DEV_ERROR;	//stop convertion	
			pRsrc->msgEn = 0;
		}
		else if(pRsrc->cvrtTimes == 0){
		}
		else
			pRsrc->cvrtTimes --;
	}
	
//	if(mcp3421_read(pRsrc, tmp) < 0)	return DEV_ERROR;
//	if(tmp[0] & 0x80){	//receive a data
//		for(i=ADC_BUF_LEN;i>1;i--)
//			pRsrc->result[i-1] = pRsrc->result[i-2];
//		res = tmp[0]&0x0f;	res >>=2;
//		pRsrc->Res = (MCP3421_Res)res;
//		gain = tmp[0]&0x03;
//		pRsrc->Gain = (MCP3421_Gain)gain;
//		if(res == SPS004_18BIT){
//			pRsrc->result[0] = tmp[3]&0x81;	
//			pRsrc->result[0] <<= 8;	pRsrc->result[0] |= tmp[2];	
//			pRsrc->result[0] <<= 8;	pRsrc->result[0] |= tmp[1];	
//		}
//		else if(res == SPS015_16BIT){
//			pRsrc->result[0] = tmp[3];	
//			pRsrc->result[0] <<= 8;	pRsrc->result[0] |= tmp[2];
//		}
//		else if(res == SPS060_14BIT){
//			pRsrc->result[0] = tmp[3]&0x9f;	
//			pRsrc->result[0] <<= 8;	pRsrc->result[0] |= tmp[2];
//		}
//		else{
//			pRsrc->result[0] = tmp[3]&0x87;	
//			pRsrc->result[0] <<= 8;	pRsrc->result[0] |= tmp[2];
//		}
//	
//		mcp3421_makeCallBackMsg(pRsrc);		
//		if(pRsrc->cvrtTimes == 1){
////			if( mcp3421_stopConvert(pRsrc)	== DEV_ERROR)	return DEV_ERROR;	//stop convertion	
//			pRsrc->msgEn = 0;
//		}
//		else if(pRsrc->cvrtTimes == 0){
//		}
//		else
//			pRsrc->cvrtTimes --;
//	}

	return DEV_SUCCESS;
}

static DEV_STATUS mcp3421_startConvert(MCP3421_Rsrc_T *pRsrc, u16 times){
	u8 tmp[4] = {0};
//	mcp3421_setMode(pRsrc, pRsrc->Res, pRsrc->Gain);
	if(mcp3421_read(pRsrc, tmp) < 0)	return DEV_ERROR;		/* First time, no use */
	pRsrc->cvrtTimes = times;
	pRsrc->msgEn = 1;
	return DEV_SUCCESS;
}
static DEV_STATUS mcp3421_stopConvert(MCP3421_Rsrc_T *pRsrc){
	u8 tmp[2] = {0, 0x06};
	if(mcp3421_write(pRsrc, tmp, 2) == 0){
		pRsrc->msgEn = 0;
		pRsrc->cvrtTimes = 0;
		return DEV_SUCCESS;
	}
	return DEV_ERROR;
}

s8 readADC(MCP3421_Rsrc_T *pRsrc, u32* result){
	u8 tmp[4], res, gain;
	
	if(mcp3421_read(pRsrc, tmp) < 0)	return -1;
	if((tmp[0] & 0x80) == 0){	//receive a data
		res = tmp[0]>>2;	res &= 0x03;
		pRsrc->Res = (MCP3421_Res)res;
		gain = tmp[0]&0x03;
		pRsrc->Gain = (MCP3421_Gain)gain;
		*result = tmp[2];   
		*result <<= 8;
		*result |= tmp[1];   
          
		if(res == SPS004_18BIT){
			*result = tmp[3]&0x03;	
			*result <<= 8;	*result |= tmp[2];	
			*result <<= 8;	*result |= tmp[1];
		}
		else if(res == SPS015_16BIT){
			*result = tmp[3];	
			*result <<= 8;	*result |= tmp[2];
		}
		else if(res == SPS060_14BIT){
			*result = tmp[3]&0x3f;	
			*result <<= 8;	*result |= tmp[2];
		}
		else{
			*result = tmp[3]&0x0f;
			*result <<= 8;	*result |= tmp[2];
		}
		
		return 1;
	}
	else 	return 0;
}

static DEV_STATUS mcp3421_offsetCal(MCP3421_Rsrc_T *pRsrc){
	u8 ms,i;
	u32 sum,result;

	if(pRsrc->Res == SPS004_18BIT)	ms = 1000/8;
	else if(pRsrc->Res == SPS015_16BIT)	ms = 1000/30;
	else if(pRsrc->Res == SPS060_14BIT)	ms = 1000/120;
	else ms = 1000/480;

	for(i=0,sum=0;i<8;i++){
		if(readADC(pRsrc, &result) == 1)	sum += result;
		HAL_Delay(ms);
	}
	sum >>= 3;
	pRsrc->Cal.offset = sum;

	pRsrc->WriteCal(pRsrc->name, pRsrc->Cal);
	
	return DEV_SUCCESS;
}

static DEV_STATUS mcp3421_gainCal(MCP3421_Rsrc_T *pRsrc, u32 trueVal){
	u8 ms,i;
	u32 sum,result;

	if(pRsrc->Res == SPS004_18BIT)	ms = 1000/8;
	else if(pRsrc->Res == SPS015_16BIT)	ms = 1000/30;
	else if(pRsrc->Res == SPS060_14BIT)	ms = 1000/120;
	else ms = 1000/480;

	for(i=0,sum=0;i<8;){
		if(readADC(pRsrc, &result) == 1){
			sum += result;
			i++;
		}
		HAL_Delay(ms);
	}
	sum >>= 3;
	if(sum > pRsrc->Cal.offset)	sum -= pRsrc->Cal.offset;
	else{ return DEV_ERROR;	}	

	pRsrc->Cal.gainMul = (trueVal*4096)/sum;
	pRsrc->Cal.gainDiv = 4096;

//	pRsrc->WriteCal(pRsrc->name, pRsrc->Cal);
	
	return DEV_SUCCESS;
}


static DEV_STATUS mcp3421_resetCal(MCP3421_Rsrc_T *pRsrc, CAL32_T cal){
	pRsrc->Cal = cal;
//	pRsrc->WriteCal(pRsrc->name, pRsrc->Cal);
	return DEV_SUCCESS;
}

static void mcp3421_resetCallBackMsg(MCP3421_Rsrc_T* pRsrc){
	u8 i;
	for(i=0;i<CALLBACK_MSG_LEN;i++)	
		pRsrc->callBackMsg[i] = 0;
}

static void mcp3421_makeCallBackMsg(MCP3421_Rsrc_T* pRsrc, u32 adcVal){
	u32 uA;
	
	if(pRsrc->SendNewMsg == NULL)	return;

	//62.5uV/LSB
	if(pRsrc->result[0] > pRsrc->Cal.offset){
		uA = ((adcVal - pRsrc->Cal.offset)*pRsrc->Cal.gainMul)/pRsrc->Cal.gainDiv;
	}
	else uA = 0;
	
	memset(pRsrc->callBackMsg, 0, CALLBACK_MSG_LEN);
	strCpy(pRsrc->callBackMsg, CALLBACK_MSG_LEN, "+msg@");
	strCatX(pRsrc->callBackMsg, pRsrc->name, ".val(", NULL);
	CONVERT.Uint32ToStrCatDecN(pRsrc->callBackMsg, uA, 8);
	strCatX(pRsrc->callBackMsg, ")", endStr, NULL);
	pRsrc->SendNewMsg(pRsrc->callBackMsg);
	
}

/*********************************************************
* Function Name  : mcp3421_read
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*********************************************************/
static s8 mcp3421_read(MCP3421_Rsrc_T* pRsrc, u8* rtnBuf){
	IIC_IO_Dev_T* iic = &pRsrc->iic;
	u8 addr;
	//address byte
     addr = 0x90 | ((pRsrc->devAddr&0x07)<<1) | 0x01;
	iic->Start(&iic->rsrc);
	iic->WriteByte(&iic->rsrc, addr);
	if(iic->WaitAck(&iic->rsrc) == 1){
		iic->Stop(&iic->rsrc);
		return -1;
	}
	//read data	
	rtnBuf[3] = iic->ReadByte(&iic->rsrc);	iic->WriteAck(&iic->rsrc);
	rtnBuf[2] = iic->ReadByte(&iic->rsrc);	iic->WriteAck(&iic->rsrc);
	rtnBuf[1] = iic->ReadByte(&iic->rsrc);	iic->WriteAck(&iic->rsrc);
	rtnBuf[0] = iic->ReadByte(&iic->rsrc);	iic->WriteAck(&iic->rsrc);
	iic->Stop(&iic->rsrc);
	return 0;
}

/*********************************************************
* Function Name  : mcp3421_write
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*********************************************************/
static s8 mcp3421_write(MCP3421_Rsrc_T* pRsrc, u8* dat, u8 len){
	u8 i;
	IIC_IO_Dev_T* iic = &pRsrc->iic;
	
	//address byte
	iic->Start(&iic->rsrc);
	//write data		
	for(i=0;i<len;i++){
		iic->WriteByte(&iic->rsrc, dat[i]);
		if(iic->WaitAck(&iic->rsrc) == 1)	{
			iic->Stop(&iic->rsrc);
			return -1;
		}
	}
	iic->Stop(&iic->rsrc);
	return 0;
}

/**********************************************************
 == THE END ==
**********************************************************/
