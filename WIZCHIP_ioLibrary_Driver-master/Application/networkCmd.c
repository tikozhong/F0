/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : networkCmd.c
* Author             : Tiko Zhong
* Date First Issued  : 12/01/2015
* Description        : This file provides a set of functions needed to manage the
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "networkCmd.h"
#include "responseX.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define FUN_LEN 64
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const u8 NETWORK_HELP[] = {
	"network commands:"
	"\n %dev.help()"
	"\n %dev.getphyconf()"
	"\n %dev.setnetinfo(indx)/()"
	"\n %dev.getnetinfo()"	
	"\n %dev.enableRaising(indx)/()"
	"\n %dev.disableRaising(indx)/()\r\n"
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
u8 networkCmd(W5500_Dev_T *pDev, PAKET_T *packetIn, PAKET_T *packetOut){
	u32 i,j,ii,jj;
	wiz_PhyConf phyConf={0};
	W5500_Rsrc_T* pRsrc = &pDev->rsrc;
	//%devName.help()
	if(isSameStr(packetIn->addr[1], (const u8*)"help") && packetIsMatch(packetIn, "%s%s")){
		packetAddTo(packetOut, "%p", NETWORK_HELP);
		return 1;
	}
	//%devName.getphyconf()
	else if(isSameStr(packetIn->addr[1], (const u8*)"getphyconf") && packetIsMatch(packetIn, "%s%s")){
		wizphy_getphyconf(&pRsrc->phyCfg);
		RESPONSE(packetOut, OK, (char*)pDev->rsrc.name, "getphyconf", "%x%x%x%x", pDev->rsrc.phyCfg.by, pDev->rsrc.phyCfg.mode, pDev->rsrc.phyCfg.speed, pDev->rsrc.phyCfg.duplex);
		return 1;
	}
	//%dev.getip()
	else if(isSameStr(packetIn->addr[1], (const u8*)"getip") && packetIsMatch(packetIn, "%s%s")){
		pDev->GetNetInfo(pRsrc);
		RESPONSE(packetOut, OK, (char*)pDev->rsrc.name, "getip", "%d%d%d%d", pRsrc->netInfo.ip[0],pRsrc->netInfo.ip[1],pRsrc->netInfo.ip[2],pRsrc->netInfo.ip[3]);
		return 1;
	}
	//%dev.setip()
	else if(isSameStr(packetIn->addr[1], (const u8*)"setip") && packetIsMatch(packetIn, "%s%s%u%u%u%u")){
		pDev->SetIp(pRsrc, *(u32*)packetIn->addr[2],*(u32*)packetIn->addr[3],*(u32*)packetIn->addr[4],*(u32*)packetIn->addr[5]);
		RESPONSE_ORG(packetOut, OK, packetIn);
		return 1;
	}	
	//%dev.getmask()
	else if(isSameStr(packetIn->addr[1], (const u8*)"getmask") && packetIsMatch(packetIn, "%s%s")){
		wizchip_getnetinfo(&pDev->rsrc.netInfo);
		RESPONSE(packetOut, OK, (char*)pDev->rsrc.name, "getip", "%d%d%d%d", pRsrc->netInfo.sn[0],pRsrc->netInfo.sn[1],pRsrc->netInfo.sn[2],pRsrc->netInfo.sn[3]);
		return 1;
	}
	//%dev.setmask()
	else if(isSameStr(packetIn->addr[1], (const u8*)"setmask") && packetIsMatch(packetIn, "%s%s%u%u%u%u")){
		pDev->SetIpMask(pRsrc, *(u32*)packetIn->addr[2],*(u32*)packetIn->addr[3],*(u32*)packetIn->addr[4],*(u32*)packetIn->addr[5]);
		RESPONSE_ORG(packetOut, OK, packetIn);
		return 1;
	}		
	//%dev.getgateway()
	else if(isSameStr(packetIn->addr[1], (const u8*)"getgateway") && packetIsMatch(packetIn, "%s%s")){
		wizchip_getnetinfo(&pDev->rsrc.netInfo);
		RESPONSE(packetOut, OK, (char*)pDev->rsrc.name, "getgateway", "%d%d%d%d", pRsrc->netInfo.gw[0],pRsrc->netInfo.gw[1],pRsrc->netInfo.gw[2],pRsrc->netInfo.gw[3]);
		return 1;
	}
	//%dev.setgateway()
	else if(isSameStr(packetIn->addr[1], (const u8*)"setgateway") && packetIsMatch(packetIn, "%s%s%u%u%u%u")){
		pDev->SetGateWay(pRsrc, *(u32*)packetIn->addr[2],*(u32*)packetIn->addr[3],*(u32*)packetIn->addr[4],*(u32*)packetIn->addr[5]);
		RESPONSE_ORG(packetOut, OK, packetIn);
		return 1;
	}		
	//%dev.getmac()
	else if(isSameStr(packetIn->addr[1], (const u8*)"getmac") && packetIsMatch(packetIn, "%s%s")){
		wizchip_getnetinfo(&pDev->rsrc.netInfo);
		RESPONSE(packetOut, OK, (char*)pDev->rsrc.name, "getmac", "%2x%2x%2x%2x%2x%2x", pRsrc->netInfo.mac[0],pRsrc->netInfo.mac[1],pRsrc->netInfo.mac[2],pRsrc->netInfo.mac[3],pRsrc->netInfo.mac[4],pRsrc->netInfo.mac[5]);
		return 1;
	}
//	//%dev.setip()
//	else if(isSameStr(packetIn->addr[1], (const u8*)"setip") && packetIsMatch(packetIn, "%s%s")){
//		wizchip_getnetinfo(&pDev->rsrc.netInfo);
//		RESPONSE(packetOut, OK, (char*)pDev->rsrc.name, "setip", "%x%x%x%x", pRsrc->netInfo.mac, pRsrc->netInfo.ip, pRsrc->netInfo.sn, pRsrc->netInfo.gw, pRsrc->netInfo.dns);
//		return 1;
//	}	
	
	else	RESPONSE_ORG(packetOut, "+err@", packetIn);
	return 0;
}

void networkMakeEventMsg(PAKET_T *packetOut, const char* DevName, u8 pinIndx, NETWORK_EVENT_T edge){
//	if(edge == INPUT_EVENT_FALLING)
//		RESPONSE(packetOut, MSG, DevName, "event", "%2d%s", pinIndx, "falling");
//	else if (edge == INPUT_EVENT_RAISING)
//		RESPONSE(packetOut, MSG, DevName, "event", "%2d%s", pinIndx, "raising");
}

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
