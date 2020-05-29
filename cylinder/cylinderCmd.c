/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : cylinderCmd.c
* Author             : Tiko Zhong
* Date First Issued  : 12/01/2015
* Description        : This file provides a set of functions needed to manage the
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "publicRsrc.h"
#include "cylinderCmd.h"
#include "responseX.h"
/* Public typedef -----------------------------------------------------------*/
//u32 cylinderCmdTick4ms;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define FUN_LEN 64
#define CMD_DEFS 4
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const char CLDR_HELP[] = {
	"Cylinder Task Commands:"
	"\n sys.help(\"CLDR_T\")"
	"\n sys.listdev(\"CLDR_T\")"	
	"\n sys.newdev(\"CLDR_T\", \"devname\", drvRst, drvAct, snsRst, snsAct)"
	"\n sys.disposedev(\"devname\")"
	"\nCylinder Device Commands:"
	"\n %devName.rename(\"newName\")"
	"\n %devName.gotorst()"
	"\n %devName.gotoact()"
	"\n %devName.gotoPos(rstPos/actPos)"
	"\n %devName.getpos()"
	"\n %devName.enabelMsg()"
	"\n %devName.disableMsg()\r\n"
};

const char DEF0_OLD[] = {"reset"};	const char DEF0_NEW[] = {"1"};
const char DEF1_OLD[] = {"action"};	const char DEF1_NEW[] = {"2"};
const char DEF2_OLD[] = {"rstpos"};	const char DEF2_NEW[] = {"1"};
const char DEF3_OLD[] = {"actpos"};	const char DEF3_NEW[] = {"2"};

CMD_DEF_T CLDR_DEFS[CMD_DEFS] = {
	{DEF0_OLD, DEF0_NEW},
	{DEF1_OLD, DEF1_NEW},
	{DEF2_OLD, DEF2_NEW},
	{DEF3_OLD, DEF3_NEW},
};

/* Private function prototypes -----------------------------------------------*/

/*******************************************************************************
* Function Name  : cylinderCmd
* Description    : gpio function commands
* Input          : - huart : using this uart to print
									: - cmdStr : the command string
* Output         : None
* Return         : None 

cylinder[n].gotoPos(reset/action)
cylinder[n].getPos(reset/action)
*******************************************************************************/
u8 cylinderCmd(CLDR_DEV_T* pDev, PAKET_T *packetIn, PAKET_T *packetOut){
	u32 timeOut;
	CLDR_RSRC_T* pRsrc = &pDev->rsrc;
	cldrPos_T pos;
	
	packetReset(packetOut);
	packetSetStyle(packetOut, PAKET_STYLE_CPP);
	//%devName.help()
	if(isSameStr(packetIn->addr[1], "help") && packetIsMatch(packetIn, "%s%s")){
		RESPONSE_ORG(packetOut, OK, packetIn);
		printS(CLDR_HELP);
		return 1;
	}
	//deviceName.enableMsg()
	else if(isSameStr(packetIn->addr[1], "enablemsg") && packetIsMatch(packetIn, "%s%s")){
		pDev->ResetCallBackMsg(&pDev->rsrc);
		pRsrc->msgEn = 1;
		RESPONSE_ORG(packetOut, OK, packetIn);
		return 1;
	}
	//deviceName.disableMsg()
	else if(isSameStr(packetIn->addr[1], "disablemsg") && packetIsMatch(packetIn, "%s%s")){
		pRsrc->msgEn = 0;
		RESPONSE_ORG(packetOut, OK, packetIn);
		return 1;
	}
//	//	deviceName.rename()
//	else if(isSameStr(packetIn->addr[1], "rename") && packetIsMatch(packetIn, "%s%s%s")){
//		strCpy(pRsrc->name, DEV_NAME_LEN, e->params[0]);
//		RESPONSE_ORG(packetIn, OK, packetOut);
//		return 1;
//	}

	//cldr.gotoPos(pos)
	else if(isSameStr(packetIn->addr[1], "gotopos") && packetIsMatch(packetIn, "%s%s%u")){
		pos = (cldrPos_T)*(u32*)packetIn->addr[0];
		pDev->GotoPos(pRsrc, pos, 5000);
		RESPONSE_ORG(packetOut, OK, packetIn);
		return 1;
	}	
	//cldr.gotoPos(pos,timeout)
	else if(isSameStr(packetIn->addr[1], "gotopos") && packetIsMatch(packetIn, "%s%s%u%u")){
		pos = (cldrPos_T)*(u32*)packetIn->addr[0];
		timeOut =  *(u32*)packetIn->addr[1];
		pDev->GotoPos(pRsrc, pos, timeOut);
		RESPONSE_ORG(packetOut, OK, packetIn);
		return 1;
	}	

	//cldr.gotoRst()
	else if(isSameStr(packetIn->addr[1], "gotorst") && packetIsMatch(packetIn, "%s%s")){
		pDev->GotoPos(pRsrc, (cldrPos_T)0, 5000);
		RESPONSE_ORG(packetOut, OK, packetIn);
		return 1;
	}	

	//cldr.gotoAct()
	else if(isSameStr(packetIn->addr[1], "gotoact") && packetIsMatch(packetIn, "%s%s")){
		pDev->GotoPos(pRsrc, (cldrPos_T)1, 5000);
		RESPONSE_ORG(packetOut, OK, packetIn);
		return 1;
	}	

	//cldr.getPos()
	else if(isSameStr(packetIn->addr[1], "getpos") && packetIsMatch(packetIn, "%s%s")){
		if(pRsrc->curPos == POS_RESET)				RESPONSE(packetOut, OK, packetIn->addr[0], packetIn->addr[1], "%s", "reset");
		else if(pRsrc->curPos == POS_ACTION)	RESPONSE(packetOut, OK, packetIn->addr[0], packetIn->addr[1], "%s", "action");
		else																	RESPONSE(packetOut, OK, packetIn->addr[0], packetIn->addr[1], "%s", "moving");
		return 1;
	}		
	return 0;
}

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
