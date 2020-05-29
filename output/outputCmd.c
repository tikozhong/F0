/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : inputCmd.c
* Author             : Tiko Zhong
* Date First Issued  : 12/01/2015
* Description        : This file provides a set of functions needed to manage the
*                      
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "outputCmd.h"
#include "responseX.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define FUN_LEN 64
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const char OUTPUT_HELP[] = {
	"output commands:"
	"\n output.help()"
	"\n output.readPin()/(indx)/(indx0,indx1)"
	"\n output.writePin(status)/(indx, b)/(indx0,bit0,indx1,bit1)"
	"\n output.togglePin()/(indx)/(indx0,indx1)"
	"\n output.runPin(indx,ms)/(ms)"
	"\n output.stopPin()/(status)/(indx,b)\r\n"
};

/* Private function prototypes -----------------------------------------------*/

/*******************************************************************************
* Function Name  : outputReport
* Description    : auto report
* Input          : - huart : using this uart to print
									: - cmdStr : the command string
* Output         : None
* Return         : None 
*******************************************************************************/
//void outputReport(UART_DEV *huartDev, OUTPUT_DEV_T *pDev, const u8* RtpNo){
//	if(pDev->rsrc.flags&(1<<OUTPUT_REPORT)){
//		huartDev->PrintStr(&huartDev->Rsrc, RtpNo);
//		outputCmd(huartDev, "output.readpin(*)\r\n", pDev);
//	}
//}

/*******************************************************************************
* Function Name  : outputCmd
* Description    : gpio function commands
* Input          : - huart : using this uart to print
									: - cmdStr : the command string
* Output         : None
* Return         : None 

output.writepin(%pin,%state)
output.toggle(%pin)
*******************************************************************************/
u8 outputCmd(OUTPUT_DEV_T *pDev, PAKET_T *packetIn, PAKET_T *packetOut){
	u32 i,j,ii,jj;

	packetReset(packetOut);
	packetSetStyle(packetOut, PAKET_STYLE_CPP);
	//%devName.help()
	if(isSameStr(packetIn->addr[1], "help") && packetIsMatch(packetIn, "%s%s")){
		RESPONSE_ORG(packetOut, OK, packetIn);
		printS(OUTPUT_HELP);
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

	//%devName.writepin(status)
	else if(isSameStr(packetIn->addr[1], "writepin") && packetIsMatch(packetIn, "%s%s%u")){
		i = *(u32*)packetIn->addr[2];
		pDev->WritePinHEX(&pDev->rsrc, i);
		RESPONSE(packetOut, OK, (char*)pDev->rsrc.name, "writepin", "%x", i);
		return 1;
	}
	//%devName.writepin(indx1,status)
	else if(isSameStr(packetIn->addr[1], "writepin") && packetIsMatch(packetIn, "%s%s%u%u")){
		i = *(u32*)packetIn->addr[2];
		j = *(u32*)packetIn->addr[3];
		if(i>16 || j>16)	RESPONSE(packetOut, ERR, (char*)pDev->rsrc.name, "writepin", "%s", "param_err");
		else{
			pDev->WritePin(&pDev->rsrc, i, (OUTPUT_STATUS)j);
			RESPONSE(packetOut, OK, (char*)pDev->rsrc.name, "writepin", "%x", pDev->rsrc.status);
		}
		return 1;
	}
	//%devName.writepin(indx1,status1,indx2,status2)
	else if(isSameStr(packetIn->addr[1], "writepin") && packetIsMatch(packetIn, "%s%s%u%u%u%u")){
		i = *(u32*)packetIn->addr[2];
		j = *(u32*)packetIn->addr[3];
		ii = *(u32*)packetIn->addr[4];
		jj = *(u32*)packetIn->addr[5];
		if(i>16 || ii>16)	RESPONSE(packetOut, ERR, (char*)pDev->rsrc.name, "writepin", "%s", "param_err");
		else{
			pDev->rsrc.status &= 0xffff^(1U<<i);
			pDev->rsrc.status &= 0xffff^(1U<<ii);
			if(j)	pDev->rsrc.status |= (1U<<i);
			if(jj)	pDev->rsrc.status |= (1U<<ii);
			pDev->WritePinHEX(&pDev->rsrc, pDev->rsrc.status);
			RESPONSE(packetOut, OK, (char*)pDev->rsrc.name, "writepin", "%x", pDev->rsrc.status);
		}
		return 1;
	}
	//%devName.togglepin()
	else if(isSameStr(packetIn->addr[1], "togglepin") && packetIsMatch(packetIn, "%s%s")){
		pDev->TogglePin(&pDev->rsrc, 0xff);
		RESPONSE(packetOut, OK, (char*)pDev->rsrc.name, "togglepin", "%x", pDev->rsrc.status);
		return 1;
	}
	//%devName.togglepin(indx)
	else if(isSameStr(packetIn->addr[1], "togglepin") && packetIsMatch(packetIn, "%s%s%u")){
		pDev->TogglePin(&pDev->rsrc, *(u32*)packetIn->addr[2]);
		RESPONSE(packetOut, OK, (char*)pDev->rsrc.name, "togglepin", "%x", pDev->rsrc.status);
		return 1;
	}
	//%devName.togglepin(indx)
	else if(isSameStr(packetIn->addr[1], "togglepin") && packetIsMatch(packetIn, "%s%s%u%u")){
		pDev->TogglePin(&pDev->rsrc, *(u32*)packetIn->addr[2]);
		pDev->TogglePin(&pDev->rsrc, *(u32*)packetIn->addr[3]);
		RESPONSE(packetOut, OK, (char*)pDev->rsrc.name, "togglepin", "%x", pDev->rsrc.status);
		return 1;
	}
	
	//%devName.runPin(ms)
	else if(isSameStr(packetIn->addr[1], "runpin") && packetIsMatch(packetIn, "%s%s%u")){
		pDev->AutoTogglePin(&pDev->rsrc, 0xff, *(u32*)packetIn->addr[2]);
		RESPONSE(packetOut, OK, (char*)pDev->rsrc.name, "runpin", NULL);
		return 1;
	}
	//%devName.runPin(indx, ms)
	else if(isSameStr(packetIn->addr[1], "runpin") && packetIsMatch(packetIn, "%s%s%u%u")){
		pDev->AutoTogglePin(&pDev->rsrc, *(u32*)packetIn->addr[2], *(u32*)packetIn->addr[3]);
		RESPONSE(packetOut, OK, (char*)pDev->rsrc.name, "runpin", NULL);
		return 1;
	}
	
	//%devName.stopPin()
	else if(isSameStr(packetIn->addr[1], "stoppin") && packetIsMatch(packetIn, "%s%s")){
		pDev->StopTogglePin(&pDev->rsrc, 0xff, 0);
		RESPONSE(packetOut, OK, (char*)pDev->rsrc.name, "stoppin", NULL);
		return 1;
	}
	//%devName.stopPin(status)
	else if(isSameStr(packetIn->addr[1], "stoppin") && packetIsMatch(packetIn, "%s%s%u")){
		pDev->StopTogglePin(&pDev->rsrc, 0xff, *(u32*)packetIn->addr[2]);
		RESPONSE(packetOut, OK, (char*)pDev->rsrc.name, "stoppin", NULL);
		return 1;
	}
	//%devName.stoppin(indx, itsStatus)
	else if(isSameStr(packetIn->addr[1], "stoppin") && packetIsMatch(packetIn, "%s%s%u%u")){
		pDev->StopTogglePin(&pDev->rsrc, *(u32*)packetIn->addr[2], *(u32*)packetIn->addr[3]);
		RESPONSE(packetOut, OK, (char*)pDev->rsrc.name, "stoppin", NULL);
		return 1;
	}
	else	RESPONSE_ORG(packetOut, "+err@", packetIn);
	return 0;
}

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
