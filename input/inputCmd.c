/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : inputCmd.c
* Author             : Tiko Zhong
* Date First Issued  : 12/01/2015
* Description        : This file provides a set of functions needed to manage the
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "inputCmd.h"
#include "responseX.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define FUN_LEN 64
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const char INPUT_HELP[] = {
	"input commands:"
	"\n input.help()"
	"\n input.readPin()/(indx)/(indx0,indx1)"
	"\n input.enableFalling(indx)/()"
	"\n input.disableFalling(indx)/()"	
	"\n input.enableRaising(indx)/()"
	"\n input.disableRaising(indx)/()\r\n"
};

/* Private function prototypes -----------------------------------------------*/
/*******************************************************************************
* Function Name  : inputCmd
* Description    : gpio function commands
* Input          : - huart : using this uart to print
									: - cmdStr : the command string
* Output         : None
* Return         : None 
*******************************************************************************/
u8 inputCmd(INPUT_DEV_T *pDev, PAKET_T *packetIn, PAKET_T *packetOut){
	u32 i,j,ii,jj;
	
	packetReset(packetOut);
	packetSetStyle(packetOut, PAKET_STYLE_CPP);
	//%devName.help()
	if(isSameStr(packetIn->addr[1], "help") && packetIsMatch(packetIn, "%s%s")){
		RESPONSE_ORG(packetOut, OK, packetIn);
		printS(INPUT_HELP);
		return 1;
	}
	//%devName.readpin()
	else if(isSameStr(packetIn->addr[1], "readpin") && packetIsMatch(packetIn, "%s%s")){
		RESPONSE(packetOut, OK, (char*)pDev->rsrc.name, "readpin", "%x", pDev->rsrc.status);
		return 1;
	}
	//%devName.readpin(indx)
	else if(isSameStr(packetIn->addr[1], "readpin") && packetIsMatch(packetIn, "%s%s%u")){
		i = *(u32*)packetIn->addr[2];
		if(i>16)	RESPONSE(packetOut, ERR, (char*)pDev->rsrc.name, "readpin", "%s", "param_err");
		else{
			j = 0x01 & (pDev->rsrc.status>>i);
			RESPONSE(packetOut, OK, (char*)pDev->rsrc.name, "readpin", "%2d%d", i,j);
		}
		return 1;
	}
	//%devName.readpin(indx1,indx2)
	else if(isSameStr(packetIn->addr[1], "readpin") && packetIsMatch(packetIn, "%s%s%u%u")){
		i = *(u32*)packetIn->addr[2];
		j = *(u32*)packetIn->addr[3];
		if(i>16 || j>16)	RESPONSE(packetOut, ERR, (char*)pDev->rsrc.name, "readpin", "%s", "param_err");
		else{
			ii = 0x01 & (pDev->rsrc.status >> i);
			jj = 0x01 & (pDev->rsrc.status >> j);
			RESPONSE(packetOut, OK, (char*)pDev->rsrc.name, "readpin", "%2d%d%2d%d", i,ii,j,jj);
		}
		return 1;
	}

	//%devName.enableFalling()
	else if(isSameStr(packetIn->addr[1], "enablefalling") && packetIsMatch(packetIn, "%s%s")){
		pDev->rsrc.enableFalling = 0xffff;
		RESPONSE(packetOut, OK, (char*)pDev->rsrc.name, "enablefalling", "%x", pDev->rsrc.enableFalling);
		return 1;
	}
	//%devName.enablefalling(indx0)
	else if(isSameStr(packetIn->addr[1], "enablefalling") && packetIsMatch(packetIn, "%s%s%u")){
		i = *(u32*)packetIn->addr[2];
		if(i>16)	RESPONSE(packetOut, ERR, (char*)pDev->rsrc.name, "enablefalling", "%s", "param_err");
		else{
			pDev->rsrc.enableFalling |= 1U<<i;
			RESPONSE(packetOut, OK, (char*)pDev->rsrc.name, "enablefalling", "%x", pDev->rsrc.enableFalling);			
		}
		return 1;
	}
	//%devName.enablefalling(indx0,indx1)
	else if(isSameStr(packetIn->addr[1], "enablefalling") && packetIsMatch(packetIn, "%s%s%u%u")){
		i = *(u32*)packetIn->addr[2];
		j = *(u32*)packetIn->addr[3];
		if(i>16 || j>16)	RESPONSE(packetOut, ERR, (char*)pDev->rsrc.name, "enablefalling", "%s", "param_err");
		else{
			pDev->rsrc.enableFalling |= 1U<<i;
			pDev->rsrc.enableFalling |= 1U<<j;
			RESPONSE(packetOut, OK, (char*)pDev->rsrc.name, "enablefalling", "%x", pDev->rsrc.enableFalling);			
		}
		return 1;
	}
	//%devName.enableRaising()
	else if(isSameStr(packetIn->addr[1], "enableraising") && packetIsMatch(packetIn, "%s%s")){
		pDev->rsrc.enableRaising = 0xffff;
		RESPONSE(packetOut, OK, (char*)pDev->rsrc.name, "enableraising", "%x", pDev->rsrc.enableRaising);
		return 1;
	}
	//%devName.enableRaising(indx0)
	else if(isSameStr(packetIn->addr[1], "enableraising") && packetIsMatch(packetIn, "%s%s%u")){
		i = *(u32*)packetIn->addr[2];
		if(i>16)	RESPONSE(packetOut, ERR, (char*)pDev->rsrc.name, "enableraising", "%s", "param_err");
		else{
			pDev->rsrc.enableRaising |= 1U<<i;
			RESPONSE(packetOut, OK, (char*)pDev->rsrc.name, "enableraising", "%x", pDev->rsrc.enableRaising);			
		}
		return 1;
	}
	//%devName.enableRaising(indx0,indx1)
	else if(isSameStr(packetIn->addr[1], "enableraising") && packetIsMatch(packetIn, "%s%s%u%u")){
		i = *(u32*)packetIn->addr[2];
		j = *(u32*)packetIn->addr[3];
		if(i>16 || j>16)	RESPONSE(packetOut, ERR, (char*)pDev->rsrc.name, "enableraising", "%s", "param_err");
		else{
			pDev->rsrc.enableRaising |= 1U<<i;
			pDev->rsrc.enableRaising |= 1U<<j;
			RESPONSE(packetOut, OK, (char*)pDev->rsrc.name, "enableraising", "%x", pDev->rsrc.enableRaising);	
		}
		return 1;
	}

	//%devName.disableFalling()
	else if(isSameStr(packetIn->addr[1], "disablefalling") && packetIsMatch(packetIn, "%s%s")){
		pDev->rsrc.enableFalling = 0;
		RESPONSE(packetOut, OK, (char*)pDev->rsrc.name, "disablefalling", "%x", pDev->rsrc.enableFalling);
		return 1;
	}
	//%devName.disableFalling(indx0)
	else if(isSameStr(packetIn->addr[1], "disablefalling") && packetIsMatch(packetIn, "%s%s%u")){
		i = *(u32*)packetIn->addr[2];
		if(i>16)	RESPONSE(packetOut, ERR, (char*)pDev->rsrc.name, "disablefalling", "%s", "param_err");
		else{
			pDev->rsrc.enableFalling &= 0xffff^(1U<<i);
			RESPONSE(packetOut, OK, (char*)pDev->rsrc.name, "disablefalling", "%x", pDev->rsrc.enableFalling);			
		}
		return 1;
	}
	//%devName.disableFalling(indx0,indx1)
	else if(isSameStr(packetIn->addr[1], "disablefalling") && packetIsMatch(packetIn, "%s%s%u%u")){
		i = *(u32*)packetIn->addr[2];
		j = *(u32*)packetIn->addr[3];
		if(i>16 || j>16)	RESPONSE(packetOut, ERR, (char*)pDev->rsrc.name, "disablefalling", "%s", "param_err");
		else{
			pDev->rsrc.enableFalling &= 0xffff^(1U<<i);
			pDev->rsrc.enableFalling &= 0xffff^(1U<<j);
			RESPONSE(packetOut, OK, (char*)pDev->rsrc.name, "disablefalling", "%x", pDev->rsrc.enableFalling);			
		}
		return 1;
	}
	//%devName.disableRaising()
	else if(isSameStr(packetIn->addr[1], "disablerasing") && packetIsMatch(packetIn, "%s%s")){
		pDev->rsrc.enableRaising = 0;
		RESPONSE(packetOut, OK, (char*)pDev->rsrc.name, "disablerasing", "%x", pDev->rsrc.enableRaising);
		return 1;
	}
	//%devName.disableRaising(indx0)
	else if(isSameStr(packetIn->addr[1], "disablerasing") && packetIsMatch(packetIn, "%s%s%u")){
		i = *(u32*)packetIn->addr[2];
		if(i>16)	RESPONSE(packetOut, ERR, (char*)pDev->rsrc.name, "disablerasing", "%s", "param_err");
		else{
			pDev->rsrc.enableRaising &= 0xffff^(1U<<i);
			RESPONSE(packetOut, OK, (char*)pDev->rsrc.name, "disablerasing", "%x", pDev->rsrc.enableRaising);			
		}
		return 1;
	}
	//%devName.disableRaising(indx0,indx1)
	else if(isSameStr(packetIn->addr[1], "disablerasing") && packetIsMatch(packetIn, "%s%s%u%u")){
		i = *(u32*)packetIn->addr[2];
		j = *(u32*)packetIn->addr[3];
		if(i>16 || j>16)	RESPONSE(packetOut, ERR, (char*)pDev->rsrc.name, "disablerasing", "%s", "param_err");
		else{
			pDev->rsrc.enableRaising &= 0xffff^(1U<<i);
			pDev->rsrc.enableRaising &= 0xffff^(1U<<j);
			RESPONSE(packetOut, OK, (char*)pDev->rsrc.name, "disablerasing", "%x", pDev->rsrc.enableRaising);	
		}
		return 1;
	}
	else	RESPONSE_ORG(packetOut, "+err@", packetIn);
	return 0;
}

void inputMakeEventMsg(PAKET_T *packetOut, const char* DevName, u8 pinIndx, INPUT_EVENT_T edge){
	if(edge == INPUT_EVENT_FALLING)
		RESPONSE(packetOut, MSG, DevName, "event", "%2d%s", pinIndx, "falling");
	else if (edge == INPUT_EVENT_RAISING)
		RESPONSE(packetOut, MSG, DevName, "event", "%2d%s", pinIndx, "raising");
}

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
