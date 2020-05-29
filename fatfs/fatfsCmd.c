/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : fatfsCmd.c
* Author             : Tiko Zhong
* Date First Issued  : 12/01/2015
* Description        : This file provides a set of functions needed to manage the
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "fatfsCmd.h"
#include "responseX.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define FUN_LEN 64

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const char FS_HELP[] = {
	"FS Commands:"
	"\n fs.help()"
	"\n fs.read(\"FileName\")"
	"\n fs.readBetween(\"FileName\",\"Head\",\"Tail\")"
	"\n fs.readRecord(\"FileName\",\"Head\",\"Tail\",\"RecName\")"
	"\n fs.readRecord(\"FileName\",\"RecName\")"
	"\n fs.writeRecord(\"FileName\",\"Head\",\"Tail\",\"RecName\",\"RecContent\")"
	"\n fs.writeRecord(\"FileName\",\"RecName\",\"RecContent\")"
	"\n fs.writeEnd(\"FileName\",\"Content\")"	
	"\n fs.writeLineEnd(\"FileName\",\"Content\")"	
	"\n fs.writeHead(\"FileName\",\"Content\")"
	"\n fs.writeLineHead(\"FileName\",\"Content\")"
	"\n fs.remove(\"FileName\")"
	"\nStart:"
	"\n start.reset()"
	"\n start.main()"
	"\n start.play()"
	"\n start.stop()\r\n"
};

/* Private function prototypes -----------------------------------------------*/
/*******************************************************************************
* Function Name  : FatfsCmd
* Description    : 
* Input          : 
								 : 
* Output         : None
* Return         : None 
*******************************************************************************/
u8 FatfsCmd(FILE_DEV_T *pDev, PAKET_T *packetIn, PAKET_T *packetOut){
//	u8 funStr[FUN_LEN], len, *rtnStr, rtnStrSz;
	FILE_RSRC_T *pRsrc = &pDev->rsrc;
//	CompiLer_Dev_T eDev;
//	CompiLer_Rsrc_T* e;
//	char blck[128];
	
	packetReset(packetOut);
	packetSetStyle(packetOut, PAKET_STYLE_CPP);
	//%devName.help()
	if(isSameStr(packetIn->addr[1], "help") && packetIsMatch(packetIn, "%s%s")){
		RESPONSE_ORG(packetOut, OK, packetIn);
		printS(FS_HELP);
		return 1;
	}
	//fs.format()
	else if(isSameStr(packetIn->addr[1], "format") && packetIsMatch(packetIn, "%s%s")){
		if(pRsrc->format != NULL){
			pRsrc->format();
			RESPONSE_ORG(packetOut, OK, packetIn);
		}
		else	RESPONSE_ORG(packetOut, ERR, packetIn);
		return 1;
	}

	//fs.remove("filename")
	else if(isSameStr(packetIn->addr[1], "remove") && packetIsMatch(packetIn, "%s%s%s")){
		if(pDev->Remove(pRsrc, packetIn->addr[2]) == 0)
			RESPONSE_ORG(packetOut, OK, packetIn);
		else RESPONSE_ORG(packetOut, ERR, packetIn);
		return 1;
	}
	
	//fs.read("filename")
	else if(isSameStr(packetIn->addr[1], "read") && packetIsMatch(packetIn, "%s%s%s")){
		if(pDev->Read(&pDev->rsrc, packetIn->addr[2]) >= 0){
			RESPONSE_ORG(packetOut, OK, packetIn);
//			memset(blck,0,128);
//			strCpy((u8*)blck, 128, "+ok@");
//			pRsrc->name
//			strcat(blck, ".read(");
//			
//			strCatX((u8*)blck, "+ok@", packetIn->addr[0], ".");
//			strCatX((u8*)blck, packetIn->addr[1], "(\"", packetIn->addr[2]);
//			strCatX((u8*)blck, "\")\r\n", NULL, NULL);
//			
//			
//			strCatX((u8*)blck, ".read(\"", packetIn->addr[2], "\")\r\n");
//			
//			
//			blck[0] = '<';
//			strcat(blck, packetIn->addr[2]);
//			strcat(blck,  ">\n");
//			packetAddTo(packetOut, "%s%p", blck, pRsrc->pBuff);
//			memset(blck,0,32);
//			blck[0] = '<'; 	blck[1] = '/';
//			strcat(blck, packetIn->addr[2]);
//			strcat(blck,  ">\r\n");
//			packetAddTo(packetOut, "%s", blck);
//			
//			packetSetStyle(packetOut, PAKET_STYLE_ORG);

		}
		else	RESPONSE_ORG(packetOut, ERR, packetIn);
		return 1;
	}
	
//	if(eDev.isMatchFuncName(e, "read") == PASS){	//match function name
//		if(eDev.isMatchFormat(e,STR) == PASS){	//match format too
//			if(pDev->Read(&pDev->rsrc, e->params[0]) >= 0){
//				strCpy(rtnStr, rtnStrSz, "+ok@");
//				strcat((char*)rtnStr, (const char*)cmdStr);	
//				strCatX((u8*)rtnStr, "<", e->params[0], ">\n");
//				Content->addr[1] = (void*)pRsrc->pBuff;	
//				len = strlen((const char*)rtnStr)+1;
//				Content->addr[2] = (void*)&rtnStr[len];
//				strCatX((u8*)Content->addr[2], "</", e->params[0], ">\r\n");
//			}
//			else{
//				memset(rtnStr, 0, rtnStrSz);
//				RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
//			}
//		}
//		else	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
//		return 1;
//	}

//	//fs.readbetween("filename", "begin", "end")
//	if(eDev.isMatchFuncName(e, "readbetween") == PASS){	//match function name
//		if(eDev.isMatchFormat(e,STR,STR,STR) == PASS){	//match format too
//			strCpy(rtnStr, rtnStrSz, "+ok@");
//			strcat((char*)rtnStr, (const char*)funStr);
//			strcat((char*)rtnStr, (const char*)"(\"");
//			len = strlen((char*)rtnStr);
//			if(pDev->ReadBetween(pRsrc, e->params[0],  e->params[1], e->params[2], &rtnStr[len], rtnStrSz-len) ){
//				strcat((char*)rtnStr, (const char*)"\")");
//				strcat((char*)rtnStr, (const char*)endStr);
//			}
//			else{
//				memset(rtnStr, 0, rtnStrSz);
//				RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
//			}
//		}
//		else	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
//		return 1;
//	}

//	//fs.readrecord("filename", "head", "tail", "begin")
//	//fs.readrecord("filename", "begin")
//	if(eDev.isMatchFuncName(e, "readrecord") == PASS){	//match function name
//		strCpy(rtnStr, rtnStrSz, "+ok@");
//		strcat((char*)rtnStr, (const char*)funStr);
//		strcat((char*)rtnStr, (const char*)"(\"");
//		len = strlen((char*)rtnStr);
//		if(eDev.isMatchFormat(e,STR,STR,STR,STR) == PASS){	//match format too
//			sTmp = pDev->ReadRecord(pRsrc, e->params[0], e->params[1], e->params[2], e->params[3], &rtnStr[len], rtnStrSz-len);
//			if(sTmp >= 0){
//				strcat((char*)rtnStr, (const char*)"\")");
//				strcat((char*)rtnStr, (const char*)endStr);
//			}
//			else if(sTmp == -1)	RESPONSE_X(ERR, rtnStr, rtnStrSz, e->funName, &e->funName[e->funcNameIndx], "%s", "OPEN_FILE");	
//			else if(sTmp == -2)	RESPONSE_X(ERR, rtnStr, rtnStrSz, e->funName, &e->funName[e->funcNameIndx], "%s", "NOT_RECORD");	
//			else if(sTmp == -3)	RESPONSE_X(ERR, rtnStr, rtnStrSz, e->funName, &e->funName[e->funcNameIndx], "%s", "READ_FILE"); 
//			else RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
//		}
//		else if(eDev.isMatchFormat(e,STR,STR) == PASS){
//			sTmp = pDev->ReadRecord(pRsrc, e->params[0], NULL, NULL, e->params[1], &rtnStr[len], rtnStrSz-len);
//			if( sTmp >= 0){
//				strcat((char*)rtnStr, (const char*)"\")");
//				strcat((char*)rtnStr, (const char*)endStr);			
//			}
//			else if(sTmp == -1)	RESPONSE_X(ERR, rtnStr, rtnStrSz, e->funName, &e->funName[e->funcNameIndx], "%s", "OPEN_FILE");
//			else if(sTmp == -2)	RESPONSE_X(ERR, rtnStr, rtnStrSz, e->funName, &e->funName[e->funcNameIndx], "%s", "NOT_RECORD");
//			else if(sTmp == -3)	RESPONSE_X(ERR, rtnStr, rtnStrSz, e->funName, &e->funName[e->funcNameIndx], "%s", "READ_FILE");
//			else RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);			
//		}
//		else	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
//		return 1;
//	}	

//	//fs.writerecord("filename", "HEAD", "TAIL", "BEGIN", "NEWLINE")
//	//fs.writerecord("filename", "BEGIN", "NEWLINE")
//	if(eDev.isMatchFuncName(e, "writerecord") == PASS){	//match function name
//		if(eDev.isMatchFormat(e,STR,STR,STR,STR,STR) == PASS){	//match format too
//			if(pDev->WriteRecord(pRsrc, e->params[0], e->params[1], e->params[2], e->params[3], e->params[4]) >= 0)
//				RESPONSE_X(OK, rtnStr, rtnStrSz, e->funName, &e->funName[e->funcNameIndx], NULL);
//			else	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
//		}
//		else if(eDev.isMatchFormat(e,STR,STR,STR) == PASS){
//			if(pDev->WriteRecord(pRsrc, e->params[0], NULL, NULL, e->params[1], e->params[2]) >= 0)
//				RESPONSE_X(OK, rtnStr, rtnStrSz, e->funName, &e->funName[e->funcNameIndx], NULL);
//			else	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);			
//		}
//		else	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
//		return 1;
//	}
//	
//	//fatfs.writeEnd("filename","content")
//	if(eDev.isMatchFuncName(e, "writeend") == PASS){	//match function name
//		if(eDev.isMatchFormat(e,STR,STR) == PASS){	//match format too
//			pDev->WriteEnd(pRsrc, e->params[0], e->params[1], NULL);
//			strCpy(rtnStr, rtnStrSz, "+ok@");
//			strcat((char*)rtnStr, (const char*)cmdStr);
//		}
//		else	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
//		return 1;
//	}

//	//fs.writeLineEnd("filename","line")
//	if(eDev.isMatchFuncName(e, "writelineend") == PASS){	//match function name
//		if(eDev.isMatchFormat(e,STR,STR) == PASS){	//match format too
//			pDev->WriteEnd(pRsrc, e->params[0],  e->params[1], "\r\n");
//			strCpy(rtnStr, rtnStrSz, "+ok@");
//			strcat((char*)rtnStr, (const char*)cmdStr);
//		}
//		else	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
//		return 1;
//	}

//	//fatfs.writeHead("filename","content")
//	if(eDev.isMatchFuncName(e, "writehead") == PASS){	//match function name
//		if(eDev.isMatchFormat(e,STR,STR) == PASS){	//match format too
//			pDev->WriteHead(pRsrc, e->params[0], e->params[1], NULL);
//			strCpy(rtnStr, rtnStrSz, "+ok@");
//			strcat((char*)rtnStr, (const char*)cmdStr);
//		}
//		else	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
//		return 1;
//	}
//	
//	//fatfs.writeLineHead("filename","line")
//	if(eDev.isMatchFuncName(e, "writelinehead") == PASS){	//match function name
//		if(eDev.isMatchFormat(e,STR,STR) == PASS){	//match format too
//			pDev->WriteHead(pRsrc, e->params[0], e->params[1], "\r\n");
//			strCpy(rtnStr, rtnStrSz, "+ok@");
//			strcat((char*)rtnStr, (const char*)cmdStr);
//		}
//		else	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)cmdStr);
//		return 1;
//	}
	
	return 0;
}


/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
