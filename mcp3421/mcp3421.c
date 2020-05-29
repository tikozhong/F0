/**********************************************************
filename: MCP3421.c
**********************************************************/

/************************包含头文件***********************/
#include "misc.h"
#include "MCP3421.h"

/**********************************************************
 Private function
**********************************************************/
static DEV_STATUS mcp3421_setMode		(MCP3421_Rsrc_T* pRsrc, MCP3421_Res res, MCP3421_Gain gain, u8 mode);
static DEV_STATUS mcp3421_polling		(MCP3421_Rsrc_T* pRsrc, u8 tick);
static DEV_STATUS mcp3421_oneShot(MCP3421_Rsrc_T *pRsrc, MCP3421_Res res, MCP3421_Gain gain);
static DEV_STATUS mcp3421_startConvert	(MCP3421_Rsrc_T *pRsrc, MCP3421_Res res, MCP3421_Gain gain, u16 times);
static DEV_STATUS mcp3421_stopConvert	(MCP3421_Rsrc_T *pRsrc);
static DEV_STATUS mcp3421_offsetCal	(MCP3421_Rsrc_T *pRsrc, MCP3421_Res res, MCP3421_Gain gain);
static DEV_STATUS mcp3421_gainCal		(MCP3421_Rsrc_T *pRsrc, MCP3421_Res res, MCP3421_Gain gain, u32 trueVal);
static DEV_STATUS mcp3421_resetCal		(MCP3421_Rsrc_T *pRsrc, CAL32_T cal);

static void mcp3421_makeCallBackMsg(MCP3421_Rsrc_T* pRsrc, u32 adcVal);

static s8 mcp3421_read(MCP3421_Rsrc_T* pRsrc, u8* rtnBuf);
static s8 mcp3421_write(MCP3421_Rsrc_T* pRsrc, u8* dat, u8 len);

static s8 readADC(MCP3421_Rsrc_T *pRsrc, u32* result);
/**********************************************************
 Public function
**********************************************************/
s8 mcp3421DevSetup1(	
		MCP3421_Dev_T *pDev, 
		PIN_T SCL, 
		PIN_T SDA, 
    u8 devAddr,
		void (*ReadCal)		(const char* TAG, u32* val),
		void (*WriteCal)	(const char* TAG, u32 val),
		const char* KEY_OFFSET,
		const char* KEY_MUL,
		const char* KEY_DIV
){
	MCP3421_Rsrc_T *pRsrc = &pDev->rsrc;
  IIC_IO_Setup(&pRsrc->iic, SCL, SDA, miscDelayMs256);
	return (mcp3421DevSetup2(pDev, &pRsrc->iic, devAddr, ReadCal, WriteCal, KEY_OFFSET, KEY_MUL, KEY_DIV));
}

s8 mcp3421DevSetup2(	
		MCP3421_Dev_T *pDev, 
		IIC_IO_Dev_T* pIIC,  
    u8 devAddr,
		void (*ReadCal)		(const char* TAG, u32* val),
		void (*WriteCal)	(const char* TAG, u32 val),
		const char* KEY_OFFSET,
		const char* KEY_MUL,
		const char* KEY_DIV
){
	CAL32_T cal;
	u32 tmp;
	MCP3421_Rsrc_T *pRsrc = &pDev->rsrc;
  pRsrc->pIIC = pIIC;
  pRsrc->devAddr = devAddr;
               
	pRsrc->Res = SPS004_18BIT;
	pRsrc->Gain = X8;
	pRsrc->ReadCal = ReadCal;
	pRsrc->WriteCal = WriteCal;
	strcpy(pRsrc->keyCalOffset, KEY_OFFSET);
	strcpy(pRsrc->keyCalMul, KEY_MUL);
	strcpy(pRsrc->keyCalDiv, KEY_DIV);
	
	//ops
	pDev->Polling = mcp3421_polling;
	pDev->OneShot = mcp3421_oneShot;
	pDev->StartConvert = mcp3421_startConvert;
	pDev->StopConvert = mcp3421_stopConvert;
	pDev->OffsetCal = mcp3421_offsetCal;
	pDev->GainCal = mcp3421_gainCal;
	pDev->ResetCal = mcp3421_resetCal;
	//READ FROM ROM
	cal.offset = 0;
	cal.gainMul = 1;
	cal.gainDiv = 1;
	if(pRsrc->ReadCal && pRsrc->WriteCal){
		pRsrc->ReadCal(pRsrc->keyCalOffset, &tmp);
		cal.offset = tmp;
		pRsrc->ReadCal(pRsrc->keyCalDiv, &tmp);
		cal.gainDiv = tmp;
		pRsrc->ReadCal(pRsrc->keyCalMul, &tmp);
		cal.gainMul = tmp;
	}
	if(cal.gainDiv == 0 || cal.gainMul == 0){
		cal.offset = 0;
		cal.gainMul = 1;
		cal.gainDiv = 1;
		mcp3421_resetCal(pRsrc, cal);
	}
	pRsrc->Cal = cal;
	if(mcp3421_stopConvert(pRsrc) == DEV_ERROR)	
		pRsrc->error |= 0x01;

	return 0;
}
		
/*********************************************************
* Function Name  : device fine
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*********************************************************/
static DEV_STATUS mcp3421_setMode(MCP3421_Rsrc_T* pRsrc, MCP3421_Res res, MCP3421_Gain gain, u8 mode){
	u8 dat[2] = {0};
	//continue mode, res, gain as param
  dat[0] = 0xd0 | ((pRsrc->devAddr&0x07)<<1);
	dat[1] = res&0x03;	dat[1] <<=2;
	dat[1] |= (gain&0x03);
	if(mode)	dat[1] |= 0x10;     //0:one shoot	1:continue
	
	if(mcp3421_write(pRsrc, dat, 2) == 0){
		pRsrc->Res = res;
		pRsrc->Gain = gain;
		//update polling timer
		if(pRsrc->Res == SPS004_18BIT)	pRsrc->tmr = 128;
		else if(pRsrc->Res == SPS015_16BIT)	pRsrc->tmr = 32;
		else if(pRsrc->Res == SPS060_14BIT)	pRsrc->tmr = 8;
		else pRsrc->tmr = 2;
		return DEV_SUCCESS;
	}
	return DEV_ERROR;
}

static DEV_STATUS mcp3421_polling(MCP3421_Rsrc_T* pRsrc, u8 tick){
	u8 i;
	s8 tmpS8;
	u32 result;
	
	pRsrc->tick += tick;
	if(pRsrc->tick < pRsrc->tmr)	return DEV_SUCCESS;
	pRsrc->tick = 0;
	switch (pRsrc->squ){
		case 0:	//do nothing
			break;
		case 1:	//one shot
			tmpS8 = readADC(pRsrc, &result);
			if(tmpS8 < 0)	return DEV_ERROR;
			mcp3421_makeCallBackMsg(pRsrc, result);
			pRsrc->squ = 0;
			pRsrc->tmr = 100;
			break;
		case 2:	//loop continue to read
			if(pRsrc->cvrtTimes){
				tmpS8 = readADC(pRsrc, &result);
				if(tmpS8 < 0){
					pRsrc->squ = 0;
					return DEV_ERROR;
				}
				if(tmpS8 == 1){
					mcp3421_makeCallBackMsg(pRsrc, result);
					if(pRsrc->cvrtTimes != 0xffff)	pRsrc->cvrtTimes --;
				}
			}
			else{
				pRsrc->squ = 0;
				pRsrc->tmr = 100;
				if( mcp3421_stopConvert(pRsrc)	== DEV_ERROR)	return DEV_ERROR;	//stop convertion
			}
			break;
		case 3:	//offset cal
			if(pRsrc->cvrtTimes){
				tmpS8 = readADC(pRsrc, &result);
				if(tmpS8 < 0){
					pRsrc->squ = 0;
					return DEV_ERROR;
				}
				if(tmpS8 == 1){
					for(i=ADC_BUF_LEN-1;i>0;i--)    pRsrc->result[i] = pRsrc->result[i-1];
					pRsrc->result[0] = result;
					pRsrc->cvrtTimes --;
				}
			}
			else{
				for(i=0;i<ADC_BUF_LEN;i++)    result += pRsrc->result[i];
				if(result & 0x20000)	pRsrc->Cal.offset = 0;
				else	pRsrc->Cal.offset = result / ADC_BUF_LEN;
				pRsrc->squ = 0;
				pRsrc->tmr = 100;
        if(pRsrc->WriteCal){    pRsrc->WriteCal(pRsrc->keyCalOffset, pRsrc->Cal.offset);	}
				if( mcp3421_stopConvert(pRsrc)	== DEV_ERROR)	return DEV_ERROR;	//stop convertion
			}
			break;
		case 4:	//gain cal
			if(pRsrc->cvrtTimes){
				tmpS8 = readADC(pRsrc, &result);
				if(tmpS8 < 0){
					pRsrc->squ = 0;
					return DEV_ERROR;
				}
				if(tmpS8 == 1){
					for(i=ADC_BUF_LEN-1;i>0;i--)    pRsrc->result[i] = pRsrc->result[i-1];
					pRsrc->result[0] = result;
					pRsrc->cvrtTimes --;
				}
			}
			else{
				for(i=0;i<ADC_BUF_LEN;i++)    result += pRsrc->result[i];
				result -= pRsrc->Cal.offset;
				result = result / ADC_BUF_LEN;
				pRsrc->Cal.gainMul = (pRsrc->Cal.gainMul*pRsrc->Cal.gainDiv)/result;
				pRsrc->squ = 0;
				pRsrc->tmr = 100;
				if(pRsrc->WriteCal)   {
					pRsrc->WriteCal(pRsrc->keyCalMul, pRsrc->Cal.gainMul);
					pRsrc->WriteCal(pRsrc->keyCalDiv, pRsrc->Cal.gainDiv);
				}
				if( mcp3421_stopConvert(pRsrc)	== DEV_ERROR)	return DEV_ERROR;	//stop convertion
			}
			break;
		default:
			pRsrc->squ = 0;
			break;
	}
	return DEV_SUCCESS;
}

static DEV_STATUS mcp3421_oneShot(MCP3421_Rsrc_T *pRsrc, MCP3421_Res res, MCP3421_Gain gain){
	if(mcp3421_setMode(pRsrc, res, gain, 0) != DEV_SUCCESS)
		return DEV_ERROR;
	pRsrc->squ = 1;
	return DEV_SUCCESS;
}

static DEV_STATUS mcp3421_startConvert(MCP3421_Rsrc_T *pRsrc, MCP3421_Res res, MCP3421_Gain gain, u16 times){
	if(mcp3421_setMode(pRsrc, res, gain, 1) != DEV_SUCCESS)
		return DEV_ERROR;
	if(times == 0)	pRsrc->cvrtTimes = 0xffff;
	else pRsrc->cvrtTimes = times;
	pRsrc->squ = 2;
	return DEV_SUCCESS;
}

static DEV_STATUS mcp3421_stopConvert(MCP3421_Rsrc_T *pRsrc){
	if(mcp3421_setMode(pRsrc, pRsrc->Res, pRsrc->Gain, 0) == DEV_SUCCESS){	//run a one-shot to entern sleep mode
		pRsrc->cvrtTimes = 0;
		pRsrc->squ = 0;
		pRsrc->tmr = 200;
		return DEV_SUCCESS;
	}
	else return DEV_ERROR;	
}

s8 readADC(MCP3421_Rsrc_T *pRsrc, u32* result){
	u8 tmp[4], res, gain;
	
	if(mcp3421_read(pRsrc, tmp) < 0)	return -1;
	if((tmp[0] & 0x80) == 0){	//receive a data
		res = tmp[0]>>2;	res &= 0x03;
		gain = tmp[0]&0x03;
		
		if((res != pRsrc->Res) || (gain != pRsrc->Gain))	return -2;
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

static DEV_STATUS mcp3421_offsetCal(MCP3421_Rsrc_T *pRsrc, MCP3421_Res res, MCP3421_Gain gain){
     if(mcp3421_setMode(pRsrc, res, gain, 1) != DEV_SUCCESS)
          return DEV_ERROR;
     pRsrc->cvrtTimes = ADC_BUF_LEN+1;
     pRsrc->squ = 3;
     return DEV_SUCCESS;
}

static DEV_STATUS mcp3421_gainCal(MCP3421_Rsrc_T *pRsrc, MCP3421_Res res, MCP3421_Gain gain, u32 expectVal){
     if(mcp3421_setMode(pRsrc, res, gain, 1) != DEV_SUCCESS)
          return DEV_ERROR;
     pRsrc->cvrtTimes = ADC_BUF_LEN+1;
     pRsrc->squ = 4;
     pRsrc->Cal.gainMul = expectVal;
     pRsrc->Cal.gainDiv = (0xefffffff/expectVal);
     return DEV_SUCCESS;
}

static DEV_STATUS mcp3421_resetCal(MCP3421_Rsrc_T *pRsrc, CAL32_T cal){
	pRsrc->Cal = cal;
  if(pRsrc->WriteCal){
    pRsrc->WriteCal(pRsrc->keyCalOffset, pRsrc->Cal.offset);
		pRsrc->WriteCal(pRsrc->keyCalMul, pRsrc->Cal.gainMul);
		pRsrc->WriteCal(pRsrc->keyCalDiv, pRsrc->Cal.gainDiv);
	}
	
	return DEV_SUCCESS;
}

//static void mcp3421_resetCallBackMsg(MCP3421_Rsrc_T* pRsrc){
//	u8 i;
//	for(i=0;i<CALLBACK_MSG_LEN;i++)
//		pRsrc->callBackMsg[i] = 0;
//}

static void mcp3421_makeCallBackMsg(MCP3421_Rsrc_T* pRsrc, u32 adcVal){
//	u32 uA;
	s32 xOut = 0;
	char callBackMsg[CALLBACK_MSG_LEN] = {0};

	if(adcVal & 0x20000){	//negtive 
		xOut = 0;
	}
	else{	//positive
		if(adcVal > pRsrc->Cal.offset){
			xOut = ((adcVal - pRsrc->Cal.offset)*pRsrc->Cal.gainMul)/pRsrc->Cal.gainDiv;
		}
		//else xOut = 0;
	}
	
//	memset(pRsrc->callBackMsg, 0, CALLBACK_MSG_LEN);
	strCpy(callBackMsg, CALLBACK_MSG_LEN, "+msg@");
	strCatX(callBackMsg, pRsrc->name, ".val(", NULL);
	CONVERT.Uint32ToStrCatDecN(callBackMsg, xOut, 8);
	strCatX(callBackMsg, ")", endStr, NULL);
  printS(callBackMsg);
}

/*********************************************************
* Function Name  : mcp3421_read
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*********************************************************/
static s8 mcp3421_read(MCP3421_Rsrc_T* pRsrc, u8* rtnBuf){
	IIC_IO_Dev_T* iic = pRsrc->pIIC;
	u8 addr;
	//address byte
  addr = 0xd0 | ((pRsrc->devAddr&0x07)<<1) | 0x01;
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
	IIC_IO_Dev_T* iic = pRsrc->pIIC;
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
