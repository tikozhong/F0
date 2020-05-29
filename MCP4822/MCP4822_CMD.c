/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : MCP4822_CMD.c
* Author             : Tiko Zhong
* Date First Issued  : 12/01/2015
* Description        : This file provides a set of functions needed to manage the
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "MCP4822_CMD.h"
#include "responseX.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
const u8 MCP4822_HELP[] = {
	"MCP4822 Task Commands:"
	"\n sys.help(\"mcp4822_t\")"
	"\n sys.newdev(\"mcp4822_t\", \"devname\", PortIndx)"
	"\n sys.disposedev(\"devname\")"
	"\n sys.listdev(\"mcp4822_t\")"
	"\nMCP4822 device Commands:"
	"\n %devName.help()"	
	"\n %devName.rename()"	
	"\n %devName.setVolt_mV(x_mV)"	
	"\n %devName.setVolt_500uV(x_500uV)\r\n"	
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/*******************************************************************************
* Function Name  : mcp4822Cmd
* Description    : 
* Input          : 
								: 
* Output         : None
* Return         : None 
*******************************************************************************/
#define FUN_LEN 64
u8 mcp4822Cmd(MCP4822Dev_t *pDev, const u8 *cmdStr, PAKET_T *Content){
	u8 *rtnStr, rtnStrSz;
	MCP4822Rsrc_T* pRsrc;
	CompiLer_Dev_T eDev;
	CompiLer_Rsrc_T* e;
	
	compilerSetup(&eDev);
	e = &eDev.rsrc;
	if(eDev.comply(e, cmdStr, NULL, 0)<0)	return 0;
	if(eDev.isMatchDevName(e, pDev->rsrc.name) != PASS)	return 0;

	rtnStr = (u8*)Content->addr[0];
	rtnStrSz = PACKE_POOL_SZ;
	pRsrc = &pDev->rsrc;

	//%devName.help()
	if(eDev.isMatchFuncName(e, (const u8*)"help") == PASS){	//match function name0
		if(eDev.isMatchFormat(e) == PASS){	//match format too
			strCpy(rtnStr, rtnStrSz, (const u8*)"+ok@");
			strcat((char*)rtnStr, (const char*)pRsrc->name);
			strcat((char*)rtnStr, (const char*)".help()\n");
			Content->addr[1] = (void*)MCP4822_HELP;
		}
		else	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
		return 1;
	}
	
	//%devName.rename("newName")
	if(eDev.isMatchFuncName(e, (const u8*)"rename") == PASS){	//match function name
		if(eDev.isMatchFormat(e,STR) == PASS){	//match format too
			if(isDevNameAvalid(NULL, e->params[0])){
				strCpy(pRsrc->name, DEV_NAME_LEN, e->params[0]);
				RESPONSE_X(OK, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
			}
			else	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
		}
		else	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
		return 1;
	}

	//%devName.setVolt_mV(mV)
	if(eDev.isMatchFuncName(e, (const u8*)"setvolt_mv") == PASS){	//match function name
		if(eDev.isMatchFormat(e,U32,U32) == PASS){	//match format too
			pDev->SetVolt_mV(&pDev->rsrc, *(u32*)e->params[0], *(u32*)e->params[1]);
			RESPONSE_X(OK, rtnStr, rtnStrSz, e->funName, &e->funName[e->funcNameIndx], NULL);
		}
		else	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
		return 1;
	}	
	
	//%devName.setVolt_500uV(x_500uV)
	if(eDev.isMatchFuncName(e, (const u8*)"setvolt_500uv") == PASS){	//match function name
		if(eDev.isMatchFormat(e,U32,U32) == PASS){	//match format too
			pDev->SetVolt_500uV(&pDev->rsrc, *(u32*)e->params[0],*(u32*)e->params[1]);
			RESPONSE_X(OK, rtnStr, rtnStrSz, e->funName, &e->funName[e->funcNameIndx], NULL);
		}
		else	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
		return 1;
	}
	
	return 0;
}

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
