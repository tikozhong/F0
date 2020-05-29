/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : response.c
* Author             : Tiko Zhong
* Date First Issued  : 20170221
* Description        : This file provides a set of functions needed to manage the
*                      
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "myString.h"
#include "publicRsrc.h"
#include "responseX.h"
#include "stdarg.h"
//#include "myUart_os.h"
const char OK[] = {"+ok@"};
const char ERR[] = {"+err@"};
const char MSG[] = {"+msg@"};

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/*******************************************************************************
* Function Name  : command
* Description    : to execute 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

/*
%d    signed int, in decimal
%u    unsigned int, in decimal
%f    float, in decimal
%s    string
%x    signed/unsigned int, in hex
%%    '%'

%md    signed int, in decimal; fill '0' if no enough
%mu    unsigned int, in decimal
%m.nf  float, in decimal
%ms    string
%mx    signed/unsigned int, in hex
*/
s8 RESPONSE(PAKET_T *packet, const char* PREFIX, const char* DEVNAME, const char* FUNC_NAME, const char* FORMAT_ORG, ...){
	u8 i,j;	
	char *pStr = (char*)packet->pool, *pTmp;
	s8 rtn;	u32 tmpU;	s32 tmpS;	va_list ap;

	strcat(pStr,PREFIX);
	strcat(pStr,DEVNAME);
	pStr[strlen(pStr)] = '.';
	strcat(pStr,FUNC_NAME);
	pStr[strlen(pStr)] = '(';

	rtn = 0;
	va_start(ap, FORMAT_ORG);	//get last arg addr
	//"%d%5d%5.5f%8x%4x%u%s"	
  for(i=0,rtn=0;1;i++){
		if (FORMAT_ORG[i] == 0)	break;
		else if (FORMAT_ORG[i] == '%'){
			i++;
			if(FORMAT_ORG[i] == 'd'){
				tmpS = va_arg(ap, s32);
				CONVERT.Uint32ToDecStrN(&pStr[strlen(pStr)], tmpS, 0);
				pStr[strlen(pStr)] = ',';
				continue;
			}
			else if(FORMAT_ORG[i] == 'u'){
				tmpU = va_arg(ap, u32);
				CONVERT.Uint32ToDecStrN(&pStr[strlen(pStr)], tmpU, 0);
				pStr[strlen(pStr)] = ',';
				continue;
			}
			else if(FORMAT_ORG[i] == 'x'){
				tmpU = va_arg(ap, u32);
				CONVERT.Uint32ToHexStr(&pStr[strlen(pStr)],tmpU);
				pStr[strlen(pStr)] = ',';
				continue;
			}
			else if(FORMAT_ORG[i] == 's'){
				pTmp = va_arg(ap, char*);
				strcat(pStr, "\"");
				strcat(pStr, pTmp);
				strcat(pStr, "\",");
				continue;
			}
			else if(FORMAT_ORG[i] == 'p'){
//				pStr = va_arg(ap, char*);
//				packetAddTo(packet, "%p", pStr);
				continue;
			}
			else if(FORMAT_ORG[i] == 'f'){
				continue;
			}
			else if(FORMAT_ORG[i]>='0' && FORMAT_ORG[i]<='9'){
				j = FORMAT_ORG[i++] - '0';
				if(FORMAT_ORG[i] == 'd'){
					tmpS = va_arg(ap, s32);
					CONVERT.Int32ToDecStrN(&pStr[strlen(pStr)],tmpS,j);
					pStr[strlen(pStr)] = ',';
					continue;
				}
				else if(FORMAT_ORG[i] == 'u'){
					tmpU = va_arg(ap, u32);
					CONVERT.Uint32ToDecStrN(&pStr[strlen(pStr)],tmpU,j);
					pStr[strlen(pStr)] = ',';
					continue;
				}
				else if(FORMAT_ORG[i] == 'x'){
					tmpU = va_arg(ap, u32);
					if(tmpU > 0xffff)	CONVERT.Uint32ToHexStr(&pStr[strlen(pStr)],tmpU);
					else if(tmpU > 0xff){
						if(j>4)	CONVERT.Uint32ToHexStr(&pStr[strlen(pStr)],tmpU);
						else		CONVERT.Uint16ToHexStr(&pStr[strlen(pStr)],tmpU);
					}
					else{
						if(j>4)	CONVERT.Uint32ToHexStr(&pStr[strlen(pStr)],tmpU);
						else if(j>2)	CONVERT.Uint16ToHexStr(&pStr[strlen(pStr)],tmpU);
						else		CONVERT.Uint8ToHexStr(&pStr[strlen(pStr)],tmpU);
					}
					pStr[strlen(pStr)] = ',';
					continue;
				}
				else if(FORMAT_ORG[i] == 's'){
					pTmp = va_arg(ap, char*);
					strcat(pStr, "\"");
					strcat(pStr, pTmp);
					strcat(pStr, "\",");
					continue;
				}
				else if(FORMAT_ORG[i] == '.'){
				}
				else{	rtn = -3;	break;	}
			}
			else if(FORMAT_ORG[i] == '.'){
			}
			else{	rtn = -2;	break;	}
		}
		else if(FORMAT_ORG[i] == ' ' || FORMAT_ORG[i] == '\t')	continue;
		else {	rtn = -1;	break;	}
	}
	va_end(ap);
	
	if(pStr[strlen(pStr)-1] == ',')	pStr[strlen(pStr)-1] = 0;
	pStr[strlen(pStr)] = ')';
	packetSetStyle(packet, PAKET_STYLE_ORG);	//ORG style
	packetAddTo(packet, "%p", pStr);
	return rtn;
}

s8 RESPONSE_ORG(PAKET_T *packet, const char* PREFIX, const PAKET_T *PACKET){
	char str[16] = {0};
	u32 tmpU;	
	u8 i;
	s8 rtn = -1;
	//in format0, 
	strcat(str,PREFIX);
	tmpU = PACKET->format[0];
	if((tmpU & 0x07) == PACKET_STR){
		strcat(str, PACKET->addr[0]);
		packetAddTo(packet, "%s", str);
		rtn++;
	}
	else {return -1;}
	tmpU >>= PACKET_DATA_BW;
	if((tmpU & 0x07) == PACKET_STR){
		packetAddTo(packet, "%s", PACKET->addr[1]);
		rtn++;
	}
	else {return -2;}
	tmpU >>= PACKET_DATA_BW;
	
	for(i=2;i<10;i++){
		if((tmpU & 0x07) == PACKET_S32){
			packetAddTo(packet, "%d", *(s32*)PACKET->addr[i]);
			rtn++;
		}
		else if((tmpU & 0x07) == PACKET_FLT){
			packetAddTo(packet, "%f", *(u32*)PACKET->addr[i]);
			rtn++;
		}
		else if((tmpU & 0x07) == PACKET_STR){	
			packetAddTo(packet, "%s", PACKET->addr[i]);
			rtn++;
		}
		else if((tmpU & 0x07) == PACKET_U32){	
			packetAddTo(packet, "%u", *(u32*)PACKET->addr[i]);
			rtn++;
		}
		else if((tmpU & 0x07) == PACKET_HEX){	
			packetAddTo(packet, "%x", *(u32*)PACKET->addr[i]);
			rtn++;
		}
		else if((tmpU & 0x07) == PACKET_PTR){	
			packetAddTo(packet, "%p", PACKET->addr[i]);
			rtn++;
		}
		else	break;
		tmpU >>= PACKET_DATA_BW;
	}
	if(rtn != 10 )	{	return rtn;	}
	//continue, in format1
	tmpU = PACKET->format[1];
	for(i=0;i<10;i++){
		if((tmpU & 0x07) == PACKET_S32){	
			packetAddTo(packet, "%d", *(s32*)PACKET->addr[10+i]);
			rtn++;
		}
		else if((tmpU & 0x07) == PACKET_FLT){
			packetAddTo(packet, "%f", *(u32*)PACKET->addr[10+i]);
			rtn++;
		}
		else if((tmpU & 0x07) == PACKET_STR){	
			packetAddTo(packet, "%s", PACKET->addr[10+i]);
			rtn++;
		}
		else if((tmpU & 0x07) == PACKET_U32){	
			packetAddTo(packet, "%u", *(u32*)PACKET->addr[10+i]);
			rtn++;
		}
		else if((tmpU & 0x07) == PACKET_HEX){	
			packetAddTo(packet, "%x", *(u32*)PACKET->addr[10+i]);
			rtn++;
		}
		else if((tmpU & 0x07) == PACKET_PTR){	
			packetAddTo(packet, "%p", PACKET->addr[10+i]);
			rtn++;
		}
		else	break;
		tmpU >>= PACKET_DATA_BW;
	}
	return rtn;
}

/*
%d    signed int, in decimal
%u    unsigned int, in decimal
%f    float, in decimal
%s    string
%x    signed/unsigned int, in hex
%%    '%'

%md    signed int, in decimal; fill '0' if no enough
%mu    unsigned int, in decimal
%m.nf  float, in decimal
%ms    string
%mx    signed/unsigned int, in hex
*/

s8 RESPONSE_X(const char* PREFIX, char* rtnStr, u8 rtnStrSz, const char* DEVNAME, const char* FUNC_NAME, const char* FORMAT_ORG, ...){
	u8 i,j,commaEn;	
	char *pStr, *p;
	s8 rtn;	u32 tmpU;	s32 tmpS;	va_list ap;
	CONVERT_T CONVERT;
	
	memset(rtnStr,0,rtnStrSz);	
	strcat((char*)rtnStr, (const char*)PREFIX);
	if(DEVNAME==NULL && FUNC_NAME==NULL){
		strcat((char*)rtnStr, (const char*)FORMAT_ORG);
		return 0;
	}

	convertSetup(&CONVERT);
	strcat((char*)rtnStr, (const char*)DEVNAME);
	strcat((char*)rtnStr, (const char*)".");
	strcat((char*)rtnStr, (const char*)FUNC_NAME);
	strcat((char*)rtnStr, (const char*)"(");
	if(FORMAT_ORG==NULL){
		strcat((char*)rtnStr, (const char*)")");
		strcat((char*)rtnStr, (const char*)endStr);
		return 0;
	}
	p = rtnStr;
	p += strlen((const char*)rtnStr);
	commaEn = 0;
	va_start(ap, FORMAT_ORG);	//get last arg addr
	//"%d%5d%5.5f%8x%4x%u%s"	
  for(i=0,rtn=0;1;i++){
		if (FORMAT_ORG[i] == 0)	break;
		else if (FORMAT_ORG[i] == '%'){
			i++;
			if(FORMAT_ORG[i] == 'd'){
				tmpS = va_arg(ap, s32);
				if(commaEn){	*p = ',';	p++;	}
				commaEn = 1;
				CONVERT.Int32ToStrCatDecN(p,tmpS,0);
				p += strlen((const char*)p);
				continue;
			}
			else if(FORMAT_ORG[i] == 'u'){
				tmpU = va_arg(ap, u32);
				if(commaEn){	*p = ',';	p++;	}
				commaEn = 1;
				CONVERT.Uint32ToStrCatDecN(p,tmpU,0);
				p += strlen((const char*)p);
				continue;
			}
			else if(FORMAT_ORG[i] == 'x'){
				tmpU = va_arg(ap, u32);
				if(commaEn){	*p = ',';	p++;	}
				commaEn = 1;
				if(tmpU > 0xffff)	CONVERT.Uint32ToStrCatHex(p,tmpU);
				else if(tmpU > 0xff)	CONVERT.Uint16ToStrCatHex(p,tmpU);
				else CONVERT.Uint8ToStrCatHex(p,tmpU);
				p += strlen((const char*)p);
				continue;
			}
			else if(FORMAT_ORG[i] == 's'){
				pStr = va_arg(ap, char*);
				if(commaEn){	*p = ',';	p++;	}
				commaEn = 1;
				*p = '"';	p++;
				strcat((char*)p,pStr);
				p += strlen((const char*)p);
				*p = '"';	p++;
				continue;
			}
			else if(FORMAT_ORG[i] == 'f'){
			}
			else if(FORMAT_ORG[i]>='0' && FORMAT_ORG[i]<='9'){
//dev.PrintStrStr(&dev.Rsrc, "m:",rtnStr);
				j = FORMAT_ORG[i++] - '0';
				if(FORMAT_ORG[i] == 'd'){
					tmpS = va_arg(ap, s32);
					if(commaEn){	*p = ',';	p++;	}
					commaEn = 1;
					CONVERT.Int32ToStrCatDecN(p,tmpS,j);
					p += strlen((const char*)p);
//dev.PrintStrStr(&dev.Rsrc, "md:",rtnStr);
					continue;
				}
				else if(FORMAT_ORG[i] == 'u'){
					tmpU = va_arg(ap, u32);
					if(commaEn){	*p = ',';	p++;	}
					commaEn = 1;
					CONVERT.Uint32ToStrCatDecN(p,tmpU,j);
					p += strlen((const char*)p);
//dev.PrintStrStr(&dev.Rsrc, "mu:",rtnStr);
					continue;
				}
				else if(FORMAT_ORG[i] == 'x'){
					tmpU = va_arg(ap, u32);
					if(commaEn){	*p = ',';	p++;	}
					commaEn = 1;
					if(tmpU > 0xffff)	CONVERT.Uint32ToStrCatHex(p,tmpU);
					else if(tmpU > 0xff){
						if(j>4)	CONVERT.Uint32ToStrCatHex(p,tmpU);
						else		CONVERT.Uint16ToStrCatHex(p,tmpU);
					}
					else{
						if(j>4)	CONVERT.Uint32ToStrCatHex(p,tmpU);
						else if(j>2)	CONVERT.Uint16ToStrCatHex(p,tmpU);
						else		CONVERT.Uint8ToStrCatHex(p,tmpU);
					}
					p += strlen((const char*)p);
//dev.PrintStrStr(&dev.Rsrc, "mx:",rtnStr);
					continue;
				}
				else if(FORMAT_ORG[i] == 's'){
					pStr = va_arg(ap, char*);
					if(commaEn){	*p = ',';	p++;	}
					commaEn = 1;
					*p = '"';	p++;
					if(j>strlen(pStr)){
						j -= strlen(pStr);
						for(;j;j--){	*p = ' ';	p++;	}
					}
					strcat((char*)p,pStr);
					p += strlen((const char*)p);
					*p = '"';	p++;
//dev.PrintStrStr(&dev.Rsrc, "ms:",rtnStr);
					continue;
				}
				else if(FORMAT_ORG[i] == '.'){
				}
				else{	rtn = -3;	break;	}
			}
			else if(FORMAT_ORG[i] == '.'){
			}
			else{	rtn = -2;	break;	}
		}
		else if(FORMAT_ORG[i] == ' ' || FORMAT_ORG[i] == '\t')	continue;
		else {	rtn = -1;	break;	}
	}

	if(rtn >= 0){
		strcat((char*)rtnStr, (const char*)")");
		strcat((char*)rtnStr, (const char*)endStr);
	}
	else{		memset(rtnStr,0,rtnStrSz);	}
	
	va_end(ap);
	return rtn;
}

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
