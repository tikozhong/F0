/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : mcp3421_CMD.c
* Author             : Tiko Zhong
* Date First Issued  : 12/01/2015
* Description        : This file provides a set of functions needed to manage the
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "publicRsrc.h"
#include "mcp3421_CMD.h"
#include "compiler.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
//extern UART_DEV myUart;

#define MCP3421_DEF_CNT 8
CMD_DEF_T MCP3421_CMD_DEF[MCP3421_DEF_CNT] = {
	{"1x", "0"},
	{"2x", "1"},
	{"4x", "2"},
	{"8x", "3"},
	{"12b", "0"},
	{"14b", "1"},
	{"16b", "2"},
	{"18b", "3"},
};

const char MC3421_HELP[] = {
	"MCP3421 Task Commands:"
//	"\n sys.help(\"mcp3421_t\")"
//	"\n sys.newdev(\"mcp3421_t\", \"devname\", portIndx)"
//	"\n sys.disposedev(\"devname\")"
//	"\n sys.listdev(\"mcp3421_t\")"
//	"\nMCP3421 device Commands:"
//	"\n %devName.rename(\"newName\")"
	"\n %devName.startStrm(times)"
	"\n %devName.startStrm(\"gain\",\"res\",times)"	
//	"\n %devName.startBckgnd()"
	"\n %devName.stop()"
	"\n %devName.setGain(gain)"
	"\n %devName.getGain()"
	"\n %devName.setRes(res)"
	"\n %devName.getRes()"
	"\n %devName.infoCal()"	
	"\n %devName.resetcal()"
	"\n %devName.offsetCal()"
	"\n %devName.gainCal(u16 expectedVal)\r\n"
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
u8 mcp3421Cmd(MCP3421_Dev_T *pDev, PAKET_T *packetIn, PAKET_T *packetOut){
	MCP3421_Rsrc_T* pRsrc;
	u8 gain,res,i;
  CAL32_T cal;
	
	pRsrc = &pDev->rsrc;
	packetReset(packetOut);
	packetSetStyle(packetOut, PAKET_STYLE_CPP);
	//%devName.help()
	if(isSameStr(packetIn->addr[1], "help") && packetIsMatch(packetIn, "%s%s")){
		RESPONSE_ORG(packetOut, OK, packetIn);
		printS(MC3421_HELP);
		return 1;
	}
	
	//%devName.startStream("res","gain",times)
	else if(isSameStr(packetIn->addr[1], "startstrm") && packetIsMatch(packetIn, "%s%s%s%s%u")){
		res = 0xff;
		gain = 0xff;
		for(i=0;i<MCP3421_DEF_CNT;i++){
			if(isSameStr(packetIn->addr[2], MCP3421_CMD_DEF[i].KeyStr)){
				if(i<4)	gain = i;
				else	res = i-4;
			}
			else if(isSameStr(packetIn->addr[3], MCP3421_CMD_DEF[i].KeyStr)){
				if(i<4)	gain = i;
				else	res = i-4;
			}
		}
		if(res<4 && gain<4){
			if(pDev->StartConvert(pRsrc, (MCP3421_Res)res, (MCP3421_Gain)gain, *(u32*)packetIn->addr[4]) == DEV_SUCCESS)
				RESPONSE_ORG(packetOut, OK, packetIn);
			else	RESPONSE_ORG(packetOut, ERR, packetIn);	
		}
		else	RESPONSE_ORG(packetOut, ERR, packetIn);	
		return 1;
	}
	//%devName.startStream(times)     
	else if(isSameStr(packetIn->addr[1], "startstrm") && packetIsMatch(packetIn, "%s%s%u")){
			if(pDev->StartConvert(pRsrc, pRsrc->Res, pRsrc->Gain, *(u32*)packetIn->addr[2]) == DEV_SUCCESS)
					 RESPONSE_ORG(packetOut, OK, packetIn);
			else	RESPONSE_ORG(packetOut, ERR, packetIn);	
		return 1;
	}
	//%devName.startStream()
	else if(isSameStr(packetIn->addr[1], "startstrm") && packetIsMatch(packetIn, "%s%s")){
		if(pDev->StartConvert(pRsrc, pRsrc->Res, pRsrc->Gain, 0) == DEV_SUCCESS)
			RESPONSE_ORG(packetOut, OK, packetIn);
		else	RESPONSE_ORG(packetOut, ERR, packetIn);	
		return 1;
	}

	//%devName.stop()
	else if(isSameStr(packetIn->addr[1], "stop") && packetIsMatch(packetIn, "%s%s")){
		if(pDev->StopConvert(pRsrc) == DEV_SUCCESS)
			RESPONSE_ORG(packetOut, OK, packetIn);
		else	RESPONSE_ORG(packetOut, ERR, packetIn);	
		return 1;
	}
	
	//%devName.config()
	else if(isSameStr(packetIn->addr[1], "config") && packetIsMatch(packetIn, "%s%s")){
		RESPONSE(packetOut, OK, pRsrc->name, "config", "%s%s", MCP3421_CMD_DEF[pRsrc->Res+4].KeyStr, MCP3421_CMD_DEF[pRsrc->Gain].KeyStr);
		return 1;
	}

	//%devName.infoCal()
	else if(isSameStr(packetIn->addr[1], "infocal") && packetIsMatch(packetIn, "%s%s")){
		RESPONSE(packetOut, OK, pRsrc->name, "infocal", "%u%u%u", pRsrc->Cal.offset, pRsrc->Cal.gainMul, pRsrc->Cal.gainDiv);
		return 1;
	}

	//%devName.resetcal(offset, mul, div)
	else if(isSameStr(packetIn->addr[1], "resetcal") && packetIsMatch(packetIn, "%s%s%u%u%u")){
    cal.offset = *(u32*)packetIn->addr[2];
    cal.gainMul = *(u32*)packetIn->addr[3];
    cal.gainDiv = *(u32*)packetIn->addr[4];
		if(pDev->ResetCal(pRsrc, cal) == DEV_SUCCESS)
			RESPONSE_ORG(packetOut, OK, packetIn);
		else	RESPONSE_ORG(packetOut, ERR, packetIn);
		return 1;
	}

	//%devName.resetcal()
	else if(isSameStr(packetIn->addr[1], "resetcal") && packetIsMatch(packetIn, "%s%s")){
    cal.offset = 0;
    cal.gainMul = 1;
    cal.gainDiv = 1;
		if(pDev->ResetCal(pRsrc, cal) == DEV_SUCCESS)
			RESPONSE_ORG(packetOut, OK, packetIn);
		else	RESPONSE_ORG(packetOut, ERR, packetIn);
		return 1;
	}  
  
	//%devName.offsetCal("res","gain")
	else if(isSameStr(packetIn->addr[1], "offsetcal") && packetIsMatch(packetIn, "%s%s%s%s")){
		res = 0xff;
		gain = 0xff;
		for(i=0;i<MCP3421_DEF_CNT;i++){
			if(isSameStr(packetIn->addr[2], MCP3421_CMD_DEF[i].KeyStr)){
				if(i<4)	gain = i;
				else	res = i-4;
			}
			else if(isSameStr(packetIn->addr[3], MCP3421_CMD_DEF[i].KeyStr)){
				if(i<4)	gain = i;
				else	res = i-4;
			}
		}
		if(pDev->OffsetCal(pRsrc, (MCP3421_Res)res, (MCP3421_Gain)gain) == DEV_SUCCESS)
			RESPONSE_ORG(packetOut, OK, packetIn);
		else	RESPONSE_ORG(packetOut, ERR, packetIn);
		return 1;
	}

	//%devName.gainCal()
	else if(isSameStr(packetIn->addr[1], "gaincal") && packetIsMatch(packetIn, "%s%s%s%s%u")){
		res = 0xff;
		gain = 0xff;
		for(i=0;i<MCP3421_DEF_CNT;i++){
			if(isSameStr(packetIn->addr[2], MCP3421_CMD_DEF[i].KeyStr)){
				if(i<4)	gain = i;
				else	res = i-4;
			}
			else if(isSameStr(packetIn->addr[3], MCP3421_CMD_DEF[i].KeyStr)){
				if(i<4)	gain = i;
				else	res = i-4;
			}
		}
		if(pDev->GainCal(pRsrc, (MCP3421_Res)res, (MCP3421_Gain)gain, *(u32*)packetIn->addr[4]) == DEV_SUCCESS)
			RESPONSE_ORG(packetOut, OK, packetIn);
		else	RESPONSE_ORG(packetOut, ERR, packetIn);
		return 1;
	}
	
	return 0;
}

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
