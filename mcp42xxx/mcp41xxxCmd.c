/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : inputCmd.c
* Author             : Tiko Zhong
* Date First Issued  : 12/01/2015
* Description        : This file provides a set of functions needed to manage the
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "mcp41xxxCmd.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/*******************************************************************************
* Function Name  : mcp41xxxCmdHelp
* Description    : gpio function commands
* Input          : - huart : using this uart to print
									: - cmdStr : the command string
* Output         : None
* Return         : None 
*******************************************************************************/
void mcp41xxxCmdHelp(UART_DEV *huartDev){
	huartDev->PrintStr(&huartDev->Rsrc, (uint8_t*)"Variable Res.:\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (uint8_t*)"\tRes.Set(DDD)\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (uint8_t*)"\tRes.Inc(DD)\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (uint8_t*)"\tRes.Dec(DD)\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (uint8_t*)"\tRes.Get()\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (uint8_t*)"\tRes.ShutDown()\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (uint8_t*)"\tRes.WakeUp()\r\n");
}

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
u8 mcp41xxxCmd(UART_DEV *huartDev, const u8* cmdStr, MCP41XXX_Dev_T* pDev){
	uint32_t val;
	u8 i,conditions;
	
	//Res.Set(DDD)
	conditions = 0;
	if(strNCmp(cmdStr,(const u8*)"res.set(", 8)==0 )		conditions++;
	i=firstNumFrStrN(cmdStr, &val,3);	if(i && val<256)	conditions++;
	if(strNCmp(&cmdStr[i],(const u8*)")", 1)==0 )				conditions++;
	if(conditions==3){
		pDev->setPos(&pDev->rsrc, (u8)(255-val));
		huartDev->PrintStr(&huartDev->Rsrc, (uint8_t*)"+ok @");
		huartDev->PrintStr(&huartDev->Rsrc, (uint8_t*)cmdStr);
		return 1;
	}

	//Res.Dec(DD)
	conditions = 0;
	if(strNCmp(cmdStr,(const u8*)"res.dec(", 8)==0 )		conditions++;
	i=firstNumFrStrN(cmdStr, &val,2);	if(i && val<256)	conditions++;
	if(strNCmp(&cmdStr[i],(const u8*)")", 1)==0 )				conditions++;
	if(conditions==3){
		i = val;
		val += pDev->rsrc.pos;
		if(val >= 256 )	val = 255;
		pDev->setPos(&pDev->rsrc, (u8)val);
		huartDev->PrintStr(&huartDev->Rsrc, (uint8_t*)"+ok @res.dec(");
		huartDev->PrintDecN(&huartDev->Rsrc, i, 2);
		huartDev->PrintStr(&huartDev->Rsrc, (u8*)") Pos=");
		huartDev->PrintDecN(&huartDev->Rsrc, 255-pDev->rsrc.pos, 2);
		huartDev->PrintStr(&huartDev->Rsrc, (u8*)"\r\n");
		return 1;
	}

	//Res.Inc(DD)
	conditions = 0;
	if(strNCmp(cmdStr,(const u8*)"res.inc(", 8)==0 )		conditions++;
	i=firstNumFrStrN(cmdStr, &val,2);	if(i && val<256)	conditions++;
	if(strNCmp(&cmdStr[i],(const u8*)")", 1)==0 )				conditions++;
	if(conditions==3){
		i = val;
		if(pDev->rsrc.pos < val)	val = 0;
		else	val = pDev->rsrc.pos-val;
		pDev->setPos(&pDev->rsrc, (u8)val);
		huartDev->PrintStr(&huartDev->Rsrc, (u8*)"+ok @res.inc(");
		huartDev->PrintDecN(&huartDev->Rsrc, i, 2);
		huartDev->PrintStr(&huartDev->Rsrc, (u8*)") Pos=");
		huartDev->PrintDecN(&huartDev->Rsrc, 255-pDev->rsrc.pos, 2);
		huartDev->PrintStr(&huartDev->Rsrc, (u8*)"\r\n");
		return 1;
	}

	//Res.Get()
	if(strNCmp(cmdStr,(const u8*)"res.get()",9)==0 ){
		huartDev->PrintStr(&huartDev->Rsrc, (uint8_t*)"+ok @Res.Get()\t%res=");
		huartDev->PrintDecN(&huartDev->Rsrc, 255-pDev->rsrc.pos,3);
		huartDev->PrintStr(&huartDev->Rsrc, (uint8_t*)"\r\n");
		return 1;
	}
	
	//Res.ShutDown()
	if(strNCmp(cmdStr,(const u8*)"res.shutdown()",14)==0 ){
		pDev->shutDown(&pDev->rsrc);
		huartDev->PrintStr(&huartDev->Rsrc, (uint8_t*)"+ok @");
		huartDev->PrintStr(&huartDev->Rsrc, (uint8_t*)cmdStr);
		return 1;
	}
	
	//Res.WakeUp()
	if(strNCmp(cmdStr,(const u8*)"res.wakeup()",12)==0 ){
		pDev->wakeUp(&pDev->rsrc);
		huartDev->PrintStr(&huartDev->Rsrc, (uint8_t*)"+ok @");
		huartDev->PrintStr(&huartDev->Rsrc, (uint8_t*)cmdStr);
		return 1;
	}

	return 0;
}


/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
