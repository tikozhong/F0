/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : ISL29125_CMD.c
* Author             : Tiko Zhong
* Date First Issued  : 12/01/2015
* Description        : This file provides a set of functions needed to manage the
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
//#include "board.h"
//#include "publicRsrc.h"
#include "ISL29125ROM_CMD.h"

#define METHOD_RAW	0
#define METHOD_CAL	1
#define METHOD_CRS	2
#define METHOD_CAL_CRS	3

const RGB_OUTPUT_METHOD DEFAULT_METHOD = (RGB_OUTPUT_METHOD)METHOD_RAW;
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
//extern UART_DEV myUart;

#define RGB_SENSOR_DEF_CNT 8
CMD_DEF_T ISL29125_CMD_DEF[RGB_SENSOR_DEF_CNT] = {
	{"rgbl", "3"},
	{"rgbh", "7"},
	{"rl", "0"},
	{"gl", "1"},
	{"bl", "2"},
	{"rh", "4"},
	{"gh", "5"},
	{"bh", "6"},
};

const char ISL29125_HELP[] = {
	"ISL29125 Task Commands:"
	"\n sys.help(\"isl29125_t\")"
//	"\n sys.newdev(\"isl29125_t\", \"devname\", iicPortIndx)"
//	"\n sys.disposedev(\"devname\")"
	"\n sys.listdev(\"isl29125_t\")"
	"\nISL29125 device Commands:"
//	"\n %devName.rename(\"newName\")"
	"\n %dev.startStrm(mod,times)\t//start stream @mod for times(0 for unlimited), raw output"
	"\n %dev.startStrm(mod,times,method)\t//start stream @mod for times(0 for unlimited), output by method"
//	"\n %devName.startBckgnd(rgbMode)"
	"\n %dev.stop()\t//stop a stream"
	"\n %dev.infoCalLow()\t//see RL GL BL cal infomation"
	"\n %dev.infoCalHigh()\t//see RH GH BH cal infomation"	
	"\n %dev.resetcal(mod)\t//reset %mod's cal to (0,1,1)"	
	"\n %dev.resetcal()\t//reset all cal to (0,1,1)"	
	"\n %dev.resetcal(mod,off,mul,div)\t//reset %mod's cal to (off,mul,div)"
	"\n %dev.resetcal(offset,mul,div)\t//reset all cal to (off,mul,div)"	
	"\n %dev.offsetCal(mod)\t//exe offset cal on %mod"
	"\n %dev.gainCal(mod, expectedVal)\t//exe gain cal on %mod"
	"\n %mod def:"
	"\n\t0:red,375Lux\t1:green,375Lux\t2:blue,375Lux\t3:rgb-l,375Lux"
	"\n\t4:red,10000Lux\t5:green,10000Lux\t6:blue,10000Lux\t7:rgb-h,10000Lux"
	"\n %method def:"
	"\n\t0:raw\t1:apply cal\t2:apply cross"
	"\r\n"
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void generateInfoLow(char* rtnStr, u16 rtnStrSz, const char* devName, const char* FunStr, ISL291259_CAL_T calCfg);
static void generateInfoHigh(char* rtnStr, u16 rtnStrSz, const char* devName, const char* FunStr, ISL291259_CAL_T calCfg);
/*******************************************************************************
* Function Name  : ISL29125Cmd
* Description    : gpio function commands
* Input          : - huart : using this uart to print
									: - cmdStr : the command string
* Output         : None
* Return         : None 

	%devName.rename("newName")
	%devName.startStream(u16 rgbMode, u16 times)
	%devName.startBckgnd(u16 rgbMode)
	%devName.stop()
	%devName.offsetCal(u16 rgbMode)
	%devName.gainCal(u16 rgbMode, u16 expectedVal)
	%devName.resetcal()
	%devName.InfoCal()
	
*******************************************************************************/
#define FUN_LEN 64
u8 ISL29125Cmd(ISL291259_Dev_T *pDev, PAKET_T *packetIn, PAKET_T *packetOut){
	ISL291259_Rsrc_T* pRsrc;
	u32 cnvtTimes,tmpU32;
	RGB_SENSOR_MODE mode;
	CAL32_T cal;

	pRsrc = &pDev->rsrc;
	packetReset(packetOut);
	packetSetStyle(packetOut, PAKET_STYLE_CPP);
	//%devName.help()
	if(isSameStr(packetIn->addr[1], "help") && packetIsMatch(packetIn, "%s%s")){
		RESPONSE_ORG(packetOut, OK, packetIn);
		printS(ISL29125_HELP);
		return 1;
	}

	//%devName.startStrm(mode)
	else if(isSameStr(packetIn->addr[1], "startstrm") && packetIsMatch(packetIn, "%s%s%u")){
		mode = (RGB_SENSOR_MODE)*(u32*)packetIn->addr[2];
		pRsrc->outMethod = DEFAULT_METHOD;
		if(pDev->StartConvert(pRsrc, mode, 0) == DEV_SUCCESS)
			RESPONSE_ORG(packetOut, OK, packetIn);
		else	RESPONSE_ORG(packetOut, ERR, packetIn);	
		return 1;
	}	
	//%devName.startStrm(mode, u16 times)
	else if(isSameStr(packetIn->addr[1], "startstrm") && packetIsMatch(packetIn, "%s%s%u%u")){
		mode = (RGB_SENSOR_MODE)*(u32*)packetIn->addr[2];
		cnvtTimes = *(u32*)packetIn->addr[3];
		pRsrc->outMethod = DEFAULT_METHOD;
		if(pDev->StartConvert(pRsrc, mode, cnvtTimes) == DEV_SUCCESS)
			RESPONSE_ORG(packetOut, OK, packetIn);
		else	RESPONSE_ORG(packetOut, ERR, packetIn);	
		return 1;
	}
	//%devName.startStrm(mode, u16 times, method)
	else if(isSameStr(packetIn->addr[1], "startstrm") && packetIsMatch(packetIn, "%s%s%u%u%u")){
		mode = (RGB_SENSOR_MODE)*(u32*)packetIn->addr[2];
		cnvtTimes = *(u32*)packetIn->addr[3];
		pRsrc->outMethod = (RGB_OUTPUT_METHOD)*(u32*)packetIn->addr[4];
		if(pDev->StartConvert(pRsrc, mode, cnvtTimes) == DEV_SUCCESS)
			RESPONSE_ORG(packetOut, OK, packetIn);
		else	RESPONSE_ORG(packetOut, ERR, packetIn);	
		return 1;
	}
	
	//%devName.startBckgnd(mode)
	else if(isSameStr(packetIn->addr[1], "startbckgnd") && packetIsMatch(packetIn, "%s%s%u")){
		mode = (RGB_SENSOR_MODE)*(u32*)packetIn->addr[2];
		cnvtTimes = *(u32*)packetIn->addr[3];
		pRsrc->outMethod = (RGB_OUTPUT_METHOD)*(u32*)packetIn->addr[4];
		if(pDev->StartConvert(pRsrc, mode, 0) == DEV_SUCCESS){
			RESPONSE_ORG(packetOut, OK, packetIn);
			pRsrc->msgEn = 0;
		}
		else	RESPONSE_ORG(packetOut, ERR, packetIn);	
		return 1;
	}
	
	//%devName.stop()
	else if(isSameStr(packetIn->addr[1], "stop") && packetIsMatch(packetIn, "%s%s")){
		mode = (RGB_SENSOR_MODE)*(u32*)packetIn->addr[2];
		cnvtTimes = *(u32*)packetIn->addr[3];
		pRsrc->outMethod = (RGB_OUTPUT_METHOD)*(u32*)packetIn->addr[4];
		if(pDev->StopConvert(pRsrc) == DEV_SUCCESS)
			RESPONSE_ORG(packetOut, OK, packetIn);
		else	RESPONSE_ORG(packetOut, ERR, packetIn);	
		return 1;
	}
	
	//%devName.infoCalLow()
	else if(isSameStr(packetIn->addr[1], "infocallow") && packetIsMatch(packetIn, "%s%s")){
		generateInfoLow((char*)packetOut->pool, PACKE_POOL_SZ, packetIn->addr[0], packetIn->addr[1],  pRsrc->calCfg);
		packetAddTo(packetOut, "%p", packetOut->pool);
		return 1;
	}
	//%devName.infoCalHigh()
	else if(isSameStr(packetIn->addr[1], "infocalhigh") && packetIsMatch(packetIn, "%s%s")){
		generateInfoHigh((char*)packetOut->pool, PACKE_POOL_SZ, packetIn->addr[0], packetIn->addr[1],  pRsrc->calCfg);
		packetAddTo(packetOut, "%p", packetOut->pool);
		return 1;
	}	
	
	
	//%devName.resetcal()		//reset all to (0,1,1)
	else if(isSameStr(packetIn->addr[1], "resetcal") && packetIsMatch(packetIn, "%s%s")){
		cal.offset = 0;
		cal.gainMul = 1;
		cal.gainDiv = 1;
		if(pDev->ResetCal(&pDev->rsrc, RGB_LOW_RNG, cal) == DEV_SUCCESS && pDev->ResetCal(&pDev->rsrc, RGB_HIGH_RNG, cal) == DEV_SUCCESS)
			RESPONSE_ORG(packetOut, OK, packetIn);
		else	RESPONSE_ORG(packetOut, ERR, packetIn);	
		return 1;
	}
	
	//%devName.resetcal(%mod)		//reset %mod to (0,1,1)
	else if(isSameStr(packetIn->addr[1], "resetcal") && packetIsMatch(packetIn, "%s%s%u")){
		if(pDev->ResetCal(&pDev->rsrc, (RGB_SENSOR_MODE)(*(u32*)packetIn->addr[2]), cal) == DEV_SUCCESS)
			RESPONSE_ORG(packetOut, OK, packetIn);
		else	RESPONSE_ORG(packetOut, ERR, packetIn);	
		return 1;
	}
	//%devName.resetcal(%offset,%mul,%div)		//reset all with (offset, mul, div)
	else if(isSameStr(packetIn->addr[1], "resetcal") && packetIsMatch(packetIn, "%s%s%u%u%u")){
		cal.offset = *(u32*)packetIn->addr[2];
		cal.gainMul = *(u32*)packetIn->addr[3];
		cal.gainDiv = *(u32*)packetIn->addr[4];
		if(pDev->ResetCal(&pDev->rsrc, RGB_LOW_RNG, cal) != DEV_SUCCESS)	RESPONSE_ORG(packetOut, OK, packetIn);
		else if(pDev->ResetCal(&pDev->rsrc, RGB_HIGH_RNG, cal) == DEV_SUCCESS)	RESPONSE_ORG(packetOut, OK, packetIn);
		else	RESPONSE_ORG(packetOut, ERR, packetIn);	
		return 1;
	}
	//%devName.resetcal(%mod,%offset,%mul,%div)		//reset %mod with (offset, mul, div)
	else if(isSameStr(packetIn->addr[1], "resetcal") && packetIsMatch(packetIn, "%s%s%u%u%u%u")){
		cal.offset = *(u32*)packetIn->addr[3];
		cal.gainMul = *(u32*)packetIn->addr[4];
		cal.gainDiv = *(u32*)packetIn->addr[5];
		if(pDev->ResetCal(&pDev->rsrc, (RGB_SENSOR_MODE)(*(u32*)packetIn->addr[2]), cal) != DEV_SUCCESS)	RESPONSE_ORG(packetOut, OK, packetIn);
		else	RESPONSE_ORG(packetOut, ERR, packetIn);	
		return 1;
	}	
	
	//%devName.offsetcal()
	else if(isSameStr(packetIn->addr[1], "offsetcal") && packetIsMatch(packetIn, "%s%s%u")){
		tmpU32 = *(u32*)packetIn->addr[2];
		if(tmpU32==0 || tmpU32==1 || tmpU32==2 || tmpU32==4 || tmpU32==5 || tmpU32==6 ){
			if(pDev->OffsetCal(&pDev->rsrc, (RGB_SENSOR_MODE)tmpU32) == DEV_SUCCESS)
				RESPONSE_ORG(packetOut, OK, packetIn);	
			else	RESPONSE_ORG(packetOut, ERR, packetIn);	
		}
		else	RESPONSE_ORG(packetOut, ERR, packetIn);	
		return 1;
	}
	
	//%devName.gainCal()
	else if(isSameStr(packetIn->addr[1], "gaincal") && packetIsMatch(packetIn, "%s%s%u%u")){
		tmpU32 = *(u32*)packetIn->addr[2];
		if(tmpU32==0 || tmpU32==1 || tmpU32==2 || tmpU32==4 || tmpU32==5 || tmpU32==6 ){
			if(pDev->GainCal(&pDev->rsrc, (RGB_SENSOR_MODE)tmpU32, *(u32*)packetIn->addr[3]) == DEV_SUCCESS)
				RESPONSE_ORG(packetOut, OK, packetIn);	
			else RESPONSE_ORG(packetOut, ERR, packetIn);	
		}
		else	RESPONSE_ORG(packetOut, ERR, packetIn);	
		return 1;
	}
	return 0;
}

static void generateInfoLow(char* rtnStr, u16 rtnStrSz, const char* devName, const char* FunStr, ISL291259_CAL_T calCfg){
	memset(rtnStr, 0, rtnStrSz);
	strCpy(rtnStr, rtnStrSz, "+ok@");
	strCatX(rtnStr, devName, ".", FunStr);
	strcat((char*)rtnStr, "()\nR(");
	CONVERT.Uint32ToStrCatDecN(rtnStr, calCfg.RL.offset, 0);
	strcat((char*)rtnStr, ",");
	CONVERT.Uint32ToStrCatDecN(rtnStr, calCfg.RL.gainMul, 0);
	strcat((char*)rtnStr, ",");
	CONVERT.Uint32ToStrCatDecN(rtnStr,calCfg.RL.gainDiv, 0);
	strcat((char*)rtnStr, ")");

	strcat((char*)rtnStr, "\nG(");
	CONVERT.Uint32ToStrCatDecN(rtnStr, calCfg.GL.offset, 0);
	strcat((char*)rtnStr, ",");
	CONVERT.Uint32ToStrCatDecN(rtnStr, calCfg.GL.gainMul, 0);
	strcat((char*)rtnStr, ",");
	CONVERT.Uint32ToStrCatDecN(rtnStr, calCfg.GL.gainDiv, 0);
	strcat((char*)rtnStr, ")");	
	
	strcat((char*)rtnStr, "\nB(");
	CONVERT.Uint32ToStrCatDecN(rtnStr, calCfg.BL.offset, 0);
	strcat((char*)rtnStr, ",");
	CONVERT.Uint32ToStrCatDecN(rtnStr, calCfg.BL.gainMul, 0);
	strcat((char*)rtnStr, ",");
	CONVERT.Uint32ToStrCatDecN(rtnStr, calCfg.BL.gainDiv, 0);
	strcat((char*)rtnStr, ")");
	strcat((char*)rtnStr, (const char*)endStr);
}

static void generateInfoHigh(char* rtnStr, u16 rtnStrSz, const char* devName, const char* FunStr, ISL291259_CAL_T calCfg){
	memset(rtnStr, 0, rtnStrSz);
	strCpy(rtnStr, rtnStrSz, "+ok@");
	strCatX(rtnStr, devName, ".", FunStr);
	strcat((char*)rtnStr, "()\nR(");
	CONVERT.Uint32ToStrCatDecN(rtnStr, calCfg.RH.offset, 0);
	strcat((char*)rtnStr, ",");
	CONVERT.Uint32ToStrCatDecN(rtnStr, calCfg.RH.gainMul, 0);
	strcat((char*)rtnStr, ",");
	CONVERT.Uint32ToStrCatDecN(rtnStr, calCfg.RH.gainDiv, 0);
	strcat((char*)rtnStr, ")");

	strcat((char*)rtnStr, "\nG(");
	CONVERT.Uint32ToStrCatDecN(rtnStr, calCfg.GH.offset, 0);
	strcat((char*)rtnStr, ",");
	CONVERT.Uint32ToStrCatDecN(rtnStr, calCfg.GH.gainMul, 0);
	strcat((char*)rtnStr, ",");
	CONVERT.Uint32ToStrCatDecN(rtnStr, calCfg.GH.gainDiv, 0);
	strcat((char*)rtnStr, ")");	
	
	strcat((char*)rtnStr, "\nB(");
	CONVERT.Uint32ToStrCatDecN(rtnStr, calCfg.BH.offset, 0);
	strcat((char*)rtnStr, ",");
	CONVERT.Uint32ToStrCatDecN(rtnStr, calCfg.BH.gainMul, 0);
	strcat((char*)rtnStr, ",");
	CONVERT.Uint32ToStrCatDecN(rtnStr, calCfg.BH.gainDiv, 0);
	strcat((char*)rtnStr, ")");
	strcat((char*)rtnStr, (const char*)endStr);
}
/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
