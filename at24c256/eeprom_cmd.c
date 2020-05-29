/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : eeprom_cmd.c
* Author             : Tiko Zhong
* Date First Issued  : 09/04/2017
* Description        : This file provides a set of functions needed to manage the
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "eeprom_cmd.h"
#include "responseX.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
const u8 EEPROM_HELP[] = {
	"EEPROM Task Commands:"
	"\n eeprom.writeStr(\"seg\", \"content\")"
	"\n eeprom.readStr(\"seg\")"
	"\n eeprom.writeNum(\"seg\", num)"
	"\n eeprom.readNum(\"seg\")"
	"\n eeprom.format()/(initVal)"
	"\n eeprom.info()\r\n"
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

u8 eepromCmd(AT24C256C_Dev_T *pDev, PAKET_T *packetIn, PAKET_T *packetOut){
	char str[64] = {0};

	packetReset(packetOut);
	packetOut->option0 = 2;	//default, packet will as cpp style	
	//%devName.help()
	if(isSameStr(packetIn->addr[1], (const u8*)"help") && packetIsMatch(packetIn, "%s%s")){
		packetAddTo(packetOut, "%p", EEPROM_HELP);
		return 1;
	}
	//%devName.format()
	else if(isSameStr(packetIn->addr[1], (const u8*)"format")){
		if(packetIsMatch(packetIn, "%s%s")){
			if(pDev->EntireErase(&pDev->rsrc, 0) == EEPROM_SUCCESS)
				RESPONSE(packetOut, OK, (char*)pDev->rsrc.name, "format", NULL);
			else RESPONSE(packetOut, ERR, (char*)pDev->rsrc.name, "format", NULL);
		}
		else if(packetIsMatch(packetIn, "%s%s%u")){
			pDev->EntireErase(&pDev->rsrc, *(u32*)packetIn->addr[2]);
			RESPONSE(packetOut, OK, (char*)pDev->rsrc.name, "format", NULL);
		}
		else	RESPONSE(packetOut, ERR, (char*)pDev->rsrc.name, "format", NULL);
		return 1;
	}
	
	//%devName.write(addr, "content")
	else if(isSameStr(packetIn->addr[1], (const u8*)"write") && packetIsMatch(packetIn, "%s%s%u%s")){
		if(pDev->Write(&pDev->rsrc, *(u32*)packetIn->addr[2], (const u8*)packetIn->addr[3], strlen((char*)packetIn->addr[3])) == EEPROM_SUCCESS)
			RESPONSE_ORG(packetOut, OK, packetIn);
		else RESPONSE(packetOut, ERR, (char*)pDev->rsrc.name, "writestr", "%s", "return");
		return 1;
	}	
	//%devName.read(addr)
	else if(isSameStr(packetIn->addr[1], (const u8*)"read") && packetIsMatch(packetIn, "%s%s%u")){
		if(pDev->Read(&pDev->rsrc, *(u32*)packetIn->addr[2], (u8*)str, 64) == EEPROM_SUCCESS)
			RESPONSE(packetOut, OK, (char*)pDev->rsrc.name, "read", "%s",str);
		else RESPONSE(packetOut, ERR, (char*)pDev->rsrc.name, "writestr", "%s", "return");
		return 1;
	}	
	
	//%devName.writeStr("seg", "content")
	else if(isSameStr(packetIn->addr[1], (const u8*)"writestr") && packetIsMatch(packetIn, "%s%s%s%s")){
		if(pDev->WriteStr(&pDev->rsrc, *(u32*)packetIn->addr[2], (char*)packetIn->addr[3]) == EEPROM_SUCCESS)
			RESPONSE_ORG(packetOut, OK, packetIn);
		else RESPONSE(packetOut, ERR, (char*)pDev->rsrc.name, "writestr", "%s", "return");
		return 1;
	}
	//%devName.readStr("seg")
	else if(isSameStr(packetIn->addr[1], (const u8*)"readstr") && packetIsMatch(packetIn, "%s%s%u")){
		memset(str,0,64);
		if(pDev->ReadStr(&pDev->rsrc, *(u32*)packetIn->addr[2], str, 64) == EEPROM_SUCCESS)
			RESPONSE(packetOut, OK, (char*)pDev->rsrc.name, "readstr", "%u%s", *(u32*)packetIn->addr[2], str);
		else RESPONSE(packetOut, ERR, (char*)pDev->rsrc.name, "readstr", "%s", "return");
		return 1;
	}
	//%devName.writeNum()
	//%devName.readNum()	
	else	RESPONSE_ORG(packetOut, ERR, packetIn);
	return 0;
	
	

	

//	//%devName.writecell(indx,"content")
//	if(eDev.isMatchFuncName(e, (const u8*)"writecell") == PASS){	//match function name
//		if(eDev.isMatchFormat(e,U32,STR) == PASS){	//match format too
//			if(pDev->Cell16Write(pRsrc, *(u32*)e->params[0], e->params[1]) == EEPROM_SUCCESS)
//				RESPONSE_X(OK, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);			
//			else	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
//		}
//		else if(eDev.isMatchFormat(e,U32,STR,STR) == PASS){
//			for(i=0;i<2;i++){
//				if(pDev->Cell16Write(pRsrc, *(u32*)e->params[0]+i, e->params[1+i]) == EEPROM_SUCCESS){}					
//				else{	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);	break;}
//				RESPONSE_X(OK, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);	
//			}
//		}
//		else if(eDev.isMatchFormat(e,U32,STR,STR,STR) == PASS){
//			for(i=0;i<3;i++){
//				if(pDev->Cell16Write(pRsrc, *(u32*)e->params[0]+i, e->params[1+i]) == EEPROM_SUCCESS){}					
//				else{	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);	break;}
//				RESPONSE_X(OK, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
//			}
//		}
//		else if(eDev.isMatchFormat(e,U32,STR,STR,STR,STR) == PASS){
//			for(i=0;i<4;i++){
//				if(pDev->Cell16Write(pRsrc, *(u32*)e->params[0]+i, e->params[1+i]) == EEPROM_SUCCESS){}					
//				else{	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);	break;}
//				RESPONSE_X(OK, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);	
//			}
//		}
//		else	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
//		return 1;
//	}	

//	//%devName.readCell(indx)
//	if(eDev.isMatchFuncName(e, (const u8*)"readcell") == PASS){	//match function name
//		if(eDev.isMatchFormat(e,U32) == PASS){	//match format too
//			memset(cellContent,0,17);
//			if(pDev->Cell16Read(pRsrc, *(u32*)e->params[0], cellContent, 16) == EEPROM_SUCCESS)
//				RESPONSE_X(OK, rtnStr, rtnStrSz, pDev->rsrc.name, e->funName, "%s", cellContent);
//			else	RESPONSE_X(ERR, rtnStr, rtnStrSz, e->funName, &e->funName[e->funcNameIndx], "%s", "readFail");
//		}
//		else	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
//		return 1;
//	}	
//	
//	//%devName.write(addr,"content"...)
//	if(eDev.isMatchFuncName(e, (const u8*)"write") == PASS){	//match function name
//		if(eDev.isMatchFormat(e,U32,STR) == PASS){	//match format too
//			if(pDev->Write(pRsrc, *(u32*)e->params[0], e->params[1], strlen(e->params[1])) == EEPROM_SUCCESS)
//				RESPONSE_X(OK, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);			
//			else	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
//		}
////		else if(eDev.isMatchFormat(e,U32,U32) == PASS){
////			for(i=0;i<2;i++){
////				if(pDev->Cell16Write(pRsrc, *(u32*)e->params[0]+i, e->params[1+i]) == EEPROM_SUCCESS){}					
////				else{	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);	break;}
////				RESPONSE_X(OK, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);	
////			}
////		}
////		else if(eDev.isMatchFormat(e,U32,S32) == PASS){
////			for(i=0;i<3;i++){
////				if(pDev->Cell16Write(pRsrc, *(u32*)e->params[0]+i, e->params[1+i]) == EEPROM_SUCCESS){}					
////				else{	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);	break;}
////				RESPONSE_X(OK, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
////			}
////		}
////		else if(eDev.isMatchFormat(e,U32,FLT) == PASS){
////			for(i=0;i<4;i++){
////				if(pDev->Cell16Write(pRsrc, *(u32*)e->params[0]+i, e->params[1+i]) == EEPROM_SUCCESS){}					
////				else{	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);	break;}
////				RESPONSE_X(OK, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);	
////			}
////		}
//		else	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
//		return 1;
//	}	

//	//%devName.read(addr,len)
//	if(eDev.isMatchFuncName(e, (const u8*)"read") == PASS){	//match function name
//		if(eDev.isMatchFormat(e,U32,U32) == PASS){	//match format too
//			i = *(u32*)e->params[1];
//			if(i > READ_MAX)	i = READ_MAX;
//			memset(buf,0,READ_MAX);
//			if(pDev->Read(pRsrc, *(u32*)e->params[0], buf, i) == EEPROM_SUCCESS)
//				RESPONSE_X(OK, rtnStr, rtnStrSz, e->funName, &e->funName[e->funcNameIndx], "%s", buf);
//			else	RESPONSE_X(ERR, rtnStr, rtnStrSz, e->funName, &e->funName[e->funcNameIndx], "%s", "readFail");
//		}
//		else	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
//		return 1;
//	}		
//	
//	return 0;
}

u8 eepromCmdX(AT24C256C_Dev_T *pDev, const u8 *cmdStr, PAKET_T *Content){
	u8 *rtnStr, rtnStrSz, cellContent[17],i, buf[READ_MAX]={0};
	AT24C256C_Rsrc_T* pRsrc;
	CompiLer_Dev_T eDev;
	CompiLer_Rsrc_T* e;

	compilerSetup(&eDev);
	e = &eDev.rsrc;
	if(eDev.comply(e, cmdStr, NULL, 0)<0)	return 0;
	if(eDev.isMatchDevName(e, (const u8*)pDev->rsrc.name) != PASS)	return 0;
	
	rtnStr = (u8*)Content->pool;
	rtnStrSz = PACKE_POOL_SZ;
	memset(rtnStr,0,rtnStrSz);
	pRsrc = &pDev->rsrc;

	//%devName.help()
	if(eDev.isMatchFuncName(e, (const u8*)"help") == PASS){	//match function name
		if(eDev.isMatchFormat(e) == PASS){	//match format too
			strCpy(rtnStr, rtnStrSz, (const u8*)"+ok@");
			strcat((char*)rtnStr, (const char*)pRsrc->name);
			strcat((char*)rtnStr, (const char*)".help()\n");
			Content->addr[1] = (void*)EEPROM_HELP;
		}
		else	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
		return 1;
	}
	
	//%devName.rename("newName")
	if(eDev.isMatchFuncName(e, (const u8*)"newName") == PASS){	//match function name
		if(eDev.isMatchFormat(e,STR) == PASS){	//match format too
//			if(DEVNAME.Add(pRsrc->name, DEV_NAME_LEN, e->params[0])){
//				strCpy(rtnStr, rtnStrSz, (const u8*)"+ok@");
//				strCatX(rtnStr, e->funName, (const u8*)"(\"", pRsrc->name);
//				strCatX(rtnStr, (const u8*)"\")", endStr, NULL);
//			}
//			else	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
		}
		else	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
		return 1;
	}
	
	//%devName.format()
	if(eDev.isMatchFuncName(e, (const u8*)"format") == PASS){	//match function name
		if(eDev.isMatchFormat(e) == PASS){	//match format too
			if(pDev->EntireErase(&pDev->rsrc, 0) == EEPROM_SUCCESS)
				RESPONSE_X(OK, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
			else 
				RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
		}
		else if(eDev.isMatchFormat(e,U32) == PASS){
			pDev->EntireErase(&pDev->rsrc, *(u32*)e->params[0]);
			RESPONSE_X(OK, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
		}
		else	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
		return 1;
	}	

	//%devName.writecell(indx,"content")
	if(eDev.isMatchFuncName(e, (const u8*)"writecell") == PASS){	//match function name
		if(eDev.isMatchFormat(e,U32,STR) == PASS){	//match format too
			if(pDev->Cell16Write(pRsrc, *(u32*)e->params[0], e->params[1]) == EEPROM_SUCCESS)
				RESPONSE_X(OK, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);			
			else	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
		}
		else if(eDev.isMatchFormat(e,U32,STR,STR) == PASS){
			for(i=0;i<2;i++){
				if(pDev->Cell16Write(pRsrc, *(u32*)e->params[0]+i, e->params[1+i]) == EEPROM_SUCCESS){}					
				else{	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);	break;}
				RESPONSE_X(OK, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);	
			}
		}
		else if(eDev.isMatchFormat(e,U32,STR,STR,STR) == PASS){
			for(i=0;i<3;i++){
				if(pDev->Cell16Write(pRsrc, *(u32*)e->params[0]+i, e->params[1+i]) == EEPROM_SUCCESS){}					
				else{	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);	break;}
				RESPONSE_X(OK, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
			}
		}
		else if(eDev.isMatchFormat(e,U32,STR,STR,STR,STR) == PASS){
			for(i=0;i<4;i++){
				if(pDev->Cell16Write(pRsrc, *(u32*)e->params[0]+i, e->params[1+i]) == EEPROM_SUCCESS){}					
				else{	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);	break;}
				RESPONSE_X(OK, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);	
			}
		}
		else	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
		return 1;
	}	

	//%devName.readCell(indx)
	if(eDev.isMatchFuncName(e, (const u8*)"readcell") == PASS){	//match function name
		if(eDev.isMatchFormat(e,U32) == PASS){	//match format too
			memset(cellContent,0,17);
			if(pDev->Cell16Read(pRsrc, *(u32*)e->params[0], cellContent, 16) == EEPROM_SUCCESS)
				RESPONSE_X(OK, rtnStr, rtnStrSz, (const u8*)pDev->rsrc.name, e->funName, "%s", cellContent);
			else	RESPONSE_X(ERR, rtnStr, rtnStrSz, e->funName, &e->funName[e->funcNameIndx], "%s", "readFail");
		}
		else	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
		return 1;
	}	
	
	//%devName.write(addr,"content"...)
	if(eDev.isMatchFuncName(e, (const u8*)"write") == PASS){	//match function name
		if(eDev.isMatchFormat(e,U32,STR) == PASS){	//match format too
			if(pDev->Write(pRsrc, *(u32*)e->params[0], e->params[1], strlen(e->params[1])) == EEPROM_SUCCESS)
				RESPONSE_X(OK, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);			
			else	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
		}
//		else if(eDev.isMatchFormat(e,U32,U32) == PASS){
//			for(i=0;i<2;i++){
//				if(pDev->Cell16Write(pRsrc, *(u32*)e->params[0]+i, e->params[1+i]) == EEPROM_SUCCESS){}					
//				else{	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);	break;}
//				RESPONSE_X(OK, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);	
//			}
//		}
//		else if(eDev.isMatchFormat(e,U32,S32) == PASS){
//			for(i=0;i<3;i++){
//				if(pDev->Cell16Write(pRsrc, *(u32*)e->params[0]+i, e->params[1+i]) == EEPROM_SUCCESS){}					
//				else{	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);	break;}
//				RESPONSE_X(OK, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
//			}
//		}
//		else if(eDev.isMatchFormat(e,U32,FLT) == PASS){
//			for(i=0;i<4;i++){
//				if(pDev->Cell16Write(pRsrc, *(u32*)e->params[0]+i, e->params[1+i]) == EEPROM_SUCCESS){}					
//				else{	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);	break;}
//				RESPONSE_X(OK, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);	
//			}
//		}
		else	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
		return 1;
	}	

	//%devName.read(addr,len)
	if(eDev.isMatchFuncName(e, (const u8*)"read") == PASS){	//match function name
		if(eDev.isMatchFormat(e,U32,U32) == PASS){	//match format too
			i = *(u32*)e->params[1];
			if(i > READ_MAX)	i = READ_MAX;
			memset(buf,0,READ_MAX);
			if(pDev->Read(pRsrc, *(u32*)e->params[0], buf, i) == EEPROM_SUCCESS)
				RESPONSE_X(OK, rtnStr, rtnStrSz, e->funName, &e->funName[e->funcNameIndx], "%s", buf);
			else	RESPONSE_X(ERR, rtnStr, rtnStrSz, e->funName, &e->funName[e->funcNameIndx], "%s", "readFail");
		}
		else	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
		return 1;
	}		
	
	return 0;
}

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
