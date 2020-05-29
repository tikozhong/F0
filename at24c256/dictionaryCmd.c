/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : dictionaryCmd.c
* Author             : Tiko Zhong
* Date First Issued  : 09/04/2017
* Description        : This file provides a set of functions needed to manage the
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "dictionaryCmd.h"
#include "responseX.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
const char DICT_HELP[] = {
	"Dictionary Task Commands:"
	"\n KEY-4BYTES MAX\tVAL-24BYTES MAX"
	"\n dict.write(\"KEY\", \"VAL\") //write KEY's value by VAL. "
	"\n dict.read() //read all KEY's value"	
	"\n dict.read(\"KEY\") //read KEY's value"
	"\n dict.readx(\"KEY\", cycleTimes) //read KEY's value, return program times"
	"\n dict.remove(\"KEY\") //remove KEY from dictionary"
	"\n dict.removeAll() //remove all and format dictionary"	
	"\r\n"
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/*******************************************************************************
* Function Name  : ISL29125Cmd
* Description    : 
* Input          : 
								: 
* Output         : None
* Return         : None 
*******************************************************************************/
//#define FUN_LEN 64
#define READ_MAX 64

u8 dictionaryCmd(DictDev_T *pDev, PAKET_T *packetIn, PAKET_T *packetOut){
	char str[64] = {0};
	u32 cycTz;

	packetReset(packetOut);
	packetSetStyle(packetOut, PAKET_STYLE_CPP);
	//%dev.help()
	if(isSameStr(packetIn->addr[1], "help") && packetIsMatch(packetIn, "%s%s")){
		printS(DICT_HELP);		
		RESPONSE_ORG(packetOut, OK, packetIn);
		return 1;
	}
	//%dev.write(key, "content")
	else if(isSameStr(packetIn->addr[1], "write") && packetIsMatch(packetIn, "%s%s%s%s")){
		if(pDev->WriteStr(&pDev->rsrc, (const char*)packetIn->addr[2], (const char*)packetIn->addr[3], strlen((char*)packetIn->addr[3])) == DICT_RTN_PASS)
			RESPONSE_ORG(packetOut, OK, packetIn);
		else RESPONSE_ORG(packetOut, ERR, packetIn);
		return 1;
	}
	//%dev.read(key)
	else if(isSameStr(packetIn->addr[1], "read") && packetIsMatch(packetIn, "%s%s%s")){
		if(pDev->ReadStr(&pDev->rsrc, packetIn->addr[2], str, 64, &cycTz) == DICT_RTN_PASS)
			RESPONSE(packetOut, OK, packetIn->addr[0], packetIn->addr[1], "%s%s", packetIn->addr[2], str);
		else RESPONSE(packetOut, ERR, packetIn->addr[0], packetIn->addr[1], "%s", "not_key");
		return 1;
	}
	//%dev.read()
	else if(isSameStr(packetIn->addr[1], "read") && packetIsMatch(packetIn, "%s%s")){
		RESPONSE_ORG(packetOut, OK, packetIn);
		pDev->ReadStrOut(&pDev->rsrc);
		return 1;
	}
	//%dev.readx(key)
	else if(isSameStr(packetIn->addr[1], "readx") && packetIsMatch(packetIn, "%s%s%s")){
		if(pDev->ReadStr(&pDev->rsrc, packetIn->addr[2], str, 64, &cycTz) == DICT_RTN_PASS)
			RESPONSE(packetOut, OK, packetIn->addr[0], packetIn->addr[1], "%s%s%u", packetIn->addr[2],str, cycTz);
		else RESPONSE(packetOut, ERR, packetIn->addr[0], packetIn->addr[1], "%s", "not_key");
		return 1;
	}
		
	else if(isSameStr(packetIn->addr[1], "remove") && packetIsMatch(packetIn, "%s%s%s")){
		if(pDev->Remove(&pDev->rsrc, packetIn->addr[2])==DICT_RTN_PASS)
			RESPONSE_ORG(packetOut, OK, packetIn);
		else 
			RESPONSE_ORG(packetOut, ERR, packetIn);		
		return 1;
	}
	else if(isSameStr(packetIn->addr[1], "removeall") && packetIsMatch(packetIn, "%s%s")){
		if(pDev->RemoveAll(&pDev->rsrc)==DICT_RTN_PASS)	RESPONSE_ORG(packetOut, OK, packetIn);
		else 	RESPONSE_ORG(packetOut, ERR, packetIn);		
		return 1;
	}		
	else	RESPONSE_ORG(packetOut, ERR, packetIn);
	return 0;
}

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
