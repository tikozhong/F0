/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : parammeter.c
* Author             : Tiko Zhong
* Date First Issued  : 05/09/2017
* Description        : This file provides a set of functions needed to manage the
*
*******************************************************************************/
#include "stdio.h"
#include "stdlib.h"

#include "mystring.h"
#include "convert.h"
#include "packet.h"
//#include "myuart_os.h"
#include "stdarg.h"
#include "string.h"
#include "publicRsrc.h"

/* using external variables ------------------------------------*/
#define COMPLIER_PARAM_LEN	32

/* global variables --------------------------------------------*/
/* private variables --------------------------------------------*/
/* private Functions ---------------------------------------------*/
static void packetPushFormat(u32* format, u8 dataType);

/*******************************************************************************
* Function Name  : setup a compiler device
* Description    : 
* Input          : pRsrc
								 : 
* Output         : None
* Return         : <0			-can not match function name
								 : >=0		-the count of params
*******************************************************************************/
void packetReset(PAKET_T* pPacket){
	memset(pPacket,0,sizeof(PAKET_T));
}

void packetSetStyle(PAKET_T* pPacket, PAKET_STYLE_T s){
	pPacket->style = s;
}

u8 packetIsMatch(PAKET_T* pPacket, const char* FORMAT){
	u8 i;
	u32 format[2] = {0};
	
	if (FORMAT == NULL)	return 0;	
	//"%d%f%s%u%x%p"
  for(i=0;1;i++){
		if (FORMAT[i] == 0)	break;
		else if (FORMAT[i] == '%'){
			i++;
			if(FORMAT[i] == 'd')	packetPushFormat(format, PACKET_S32);
			else if(FORMAT[i] == 'f')	packetPushFormat(format, PACKET_FLT);
			else if(FORMAT[i] == 's')	packetPushFormat(format, PACKET_STR);
			else if(FORMAT[i] == 'u')	packetPushFormat(format, PACKET_U32);
			else if(FORMAT[i] == 'x')	packetPushFormat(format, PACKET_HEX);
			else if(FORMAT[i] == 'p')	packetPushFormat(format, PACKET_PTR);
			else{	return 0;}
		}
		else if(FORMAT[i] == ' ' || FORMAT[i] == '\t')	continue;
		else {	return 0;	}
	}
	if((pPacket->format[0] == format[0])	&& (pPacket->format[1] == format[1]))	return 1;
	return 0;
}

static void packetPushFormat(u32* format, u8 dataType){
//	pPacket->format0 |= (u32)PACKET_S32<<(argIndx*2);
	u32 tmpU;	
	u8 i=0;
	//in format0, 
	tmpU = format[0];
	for(i=0;i<10;i++){
		if((tmpU & 0x07) == 0){	//this is it
			format[0] |= (dataType)<<(i*PACKET_DATA_BW);
			return;
		}
		tmpU >>= PACKET_DATA_BW;
	}
	//continue, in format1
	tmpU = format[1];
	for(i=0;i<10;i++){
		if((tmpU & 0x07) == 0){	//this is it
			format[1] |= (dataType)<<(i*PACKET_DATA_BW);
			return;
		}
		tmpU >>= PACKET_DATA_BW;
	}
}

//seek to the last argIndx
static u8 packetLastArgIndx(PAKET_T* pPacket){
	u32 tmpU;	
	u8 argIndx=0,i=0;
	//in format0, 
	tmpU = pPacket->format[0];
	for(i=0;i<10;i++){
		if(tmpU & 0x07){	//use 3bit to respect a data
			argIndx++;
			tmpU >>= PACKET_DATA_BW;
		}
		else {	return argIndx;	}
	}
	//continue, in format1
	tmpU = pPacket->format[1];
	for(i=0;i<10;i++){
		if(tmpU & 0x07){	//use 3bit to respect a data
			argIndx++;
			tmpU >>= PACKET_DATA_BW;
		}
		else {	return argIndx;	}
	}
	return argIndx;
}

//#define PACKET_NULL	0	//emptpy
//#define PACKET_S32	1	//%d	有符号整形
//#define PACKET_FLT	2	//%f	浮点数
//#define PACKET_STR	3	//%s	字符串
//#define PACKET_U32	4	//%u	无符号整型
//#define PACKET_X32	5	//%x	无符号16进制
//#define PACKET_PTR	6	//%p	字符串指针
s8 packetAddTo(PAKET_T* pPacket, const char* FORMAT, ...){
	va_list ap;
	u32 tmpU, *pU32;	
	u8 i,j;	s8 rtn, argIndx;
	char *pStr;

	argIndx = packetLastArgIndx(pPacket);
	va_start(ap, FORMAT);	//get last arg addr
  for(i=0,rtn=0;1;i++){
		if (FORMAT[i] == 0)	break;
		else if (FORMAT[i] == '%'){
			i++;
			if(FORMAT[i] == 's'){	//need to copy to interal pool
				pStr = va_arg(ap, char*);
				for(j=0;1;j++){
					pPacket->pool[pPacket->poolIndx+j] = pStr[j];
					if(pStr[j] == 0) 	break;
				}
				pPacket->addr[argIndx++] = &pPacket->pool[pPacket->poolIndx];
				pPacket->poolIndx += j+1;
				packetPushFormat(pPacket->format, PACKET_STR);
				continue;
			}
			else if(FORMAT[i] == 'p'){		//only add a pointer without copy any data
				pStr = va_arg(ap, char*);
				pPacket->addr[argIndx++] = pStr;
				packetPushFormat(pPacket->format, PACKET_PTR);
				continue;
			}			
			else if(FORMAT[i] == 'd' || FORMAT[i] == 'u' || FORMAT[i] == 'x'|| FORMAT[i] == 'f'){
				tmpU = va_arg(ap, u32);
				while(1){	//align to u32
					pU32 = (u32*)&pPacket->pool[pPacket->poolIndx];
					if((u32)pU32%4 == 0)	break;	
					pPacket->poolIndx++;
				}
				*pU32 = tmpU;
				pPacket->addr[argIndx++] = &pPacket->pool[pPacket->poolIndx];
				pPacket->poolIndx += 4;
				if(FORMAT[i] == 'd')	packetPushFormat(pPacket->format, PACKET_S32);
				else if(FORMAT[i] == 'u')	packetPushFormat(pPacket->format, PACKET_U32);
				else if(FORMAT[i] == 'x')	packetPushFormat(pPacket->format, PACKET_HEX);
				else if(FORMAT[i] == 'f')	packetPushFormat(pPacket->format, PACKET_FLT);
				continue;
			}
			else{	rtn = -2;	break;	}
		}
		else if(FORMAT[i] == ' ' || FORMAT[i] == '\t')	continue;
		else {	rtn = -1;	break;	}
	}
	va_end(ap);
	
	return rtn;
}

/*******************************************************************************
* Function Name  : packetRemoveFr
* Description    : 
* Input          : pPacket
								 : 
* Output         : None
* Return         : <0			-can not match function name
								 : >=0		-the count of params
*******************************************************************************/
//s8 packetRemoveFr(PAKET_T* pPacket, u8 indx){
//	u8 i,fr,to;
//	u32 tmp,x;
//	
//	if((pPacket->format[0] | pPacket->format[1]) == 0)	return -1;
//	else if((pPacket->format0 & 0x03) == 0){
//		memset(pPacket->pool, 0, PACKE_POOL_SZ);
//		packetReset(pPacket);
//		return 0;
//	}
//	else{	//more than 2 arg
//		fr = 0; 
//		to = 0;
//		for(i=0;i<PACKE_POOL_SZ;i++){
//			if(&pPacket->pool[i] == pPacket->addr[indx])	fr = i;
//			if(&pPacket->pool[i] == pPacket->addr[indx+1])	to = i;
//		}
//		
//		for(i=fr;i<to;i++)	pPacket->pool[i] = 0;
//		for(i=0;1;i++){
//			if(to+i >= PACKE_POOL_SZ)	break;
//			pPacket->pool[fr+i] = pPacket->pool[to+i];
//			pPacket->pool[to+i] = 0;
//		}
//		pPacket->poolIndx -= (to-fr);
//		for(i=indx;i<PACKE_ARG_SZ-1;i++){	
//			if(pPacket->addr[i+1] != 0)
//				pPacket->addr[i] = (void*)((u32)pPacket->addr[i+1] - (to-fr));
//			else 
//				pPacket->addr[i] = pPacket->addr[i+1];
//		}
//		pPacket->addr[PACKE_ARG_SZ-1] = 0;
//		
//		tmp = pPacket->format0>>2;
//		x = (0xffffffff<<(indx*2));
//		tmp &= x;
//		pPacket->format0 &= (x^0xffffffff);
//		pPacket->format0 |= tmp;
//	}
//	return 0;
//}


/*******************************************************************************
* Function Name  : packetToStrCpp
* Description    : 
* Input          : 
								 : 
* Output         : None
* Return         :
								 : 
*******************************************************************************/
s16 packetToStrCpp(char* str, u8 sz, PAKET_T* p){
	u8 i;
	u32 format;
	char tmp[16];
	u8 addMark = 0;	
	u8 sym;
	
	if(p == NULL && format==0)	return -1;
	memset(str,0,sz);
	format = p->format[0];
	//cpp style, dev.func(123, "abc")
	if((format & 0x3f) == ((PACKET_STR<<PACKET_DATA_BW) | PACKET_STR)){	//first two must be string
		strcat(str, p->addr[0]);
		strcat(str, ".");
		strcat(str, p->addr[1]);
		strcat(str, "(");
		format >>= (PACKET_DATA_BW+PACKET_DATA_BW);
		for(i=2;i<20;i++){ //up to 20 arguments
			if(i==10){	format = p->format[1];}	//reloard
			sym = format & 0x07;
			if(sym == PACKET_S32){
				memset(tmp,0,16);
				CONVERT.Int32ToDecStrN(tmp, *(s32*)p->addr[i], 0);
				if(addMark)	{
					strcat(str, ", ");
					addMark = 0;
				}
				strcat(str, tmp);
				addMark = 1;
			}
			else if(sym == PACKET_U32){
				memset(tmp,0,16);
				CONVERT.Uint32ToDecStrN(tmp, *(s32*)p->addr[i], 0);
				if(addMark)	{
					strcat(str, ", ");
					addMark = 0;
				}
				strcat(str, tmp);
				addMark = 1;
			}
			else if(sym == PACKET_STR){
				if(addMark)	{
					strcat(str, ", ");
					addMark = 0;
				}
				strcat(str, "\"");
				strcat(str, p->addr[i]);
				strcat(str, "\"");
				addMark = 1;
			}
			else if(sym == PACKET_PTR){
				if(addMark)	{
					strcat(str, ", ");
					addMark = 0;
				}
				strcat(str, "\"");
				strcat(str, p->addr[i]);	//if this string is too long, will float
				strcat(str, "\"");
				addMark = 1;
			}
			else if(sym == PACKET_FLT){
				memset(tmp,0,16);
				//				itoa(*(u32*)p->addr[i],tmp);
				if(addMark)	{
					strcat(str, ", ");
					addMark = 0;
				}
				strcat(str, tmp);
				addMark = 1;
			}
			else if(sym == PACKET_HEX){
				memset(tmp,0,16);
				CONVERT.Uint32ToHexStr(tmp, *(u32*)p->addr[i]);
				if(addMark)	{
					strcat(str, ", ");
					addMark = 0;
				}
				strcat(str, tmp);
				addMark = 1;
			}
			else {	break;	}
			format >>= PACKET_DATA_BW;
		}
		strcat(str, ")");
		strcat(str, (char*)endStr);
		return strlen(str);
	}
	return -100;
}

/*******************************************************************************
* Function Name  : packetToStrC
* Description    : 
* Input          : 
								 : 
* Output         : None
* Return         :
								 : 
*******************************************************************************/
s16 packetToStrC(char* str, u8 sz, PAKET_T* p){
	u8 i;
	u32 format;
	char tmp[16];
	u8 addMark = 0;	
	u8 sym;
	if(p == NULL && format==0)	return -1;
	memset(str,0,sz);
	format = p->format[0];
	//cpp style, dev.func(123, "abc")
	if((format & 0x07) == PACKET_STR){	//first must be string
		strcat(str, p->addr[0]);
		strcat(str, "(");
		format >>= (PACKET_DATA_BW);
		for(i=1;i<20;i++){ //up to 20 arguments
			if(i==10){	format = p->format[1];}	//reloard
			sym = format & 0x07;
			if(sym == PACKET_S32){
				memset(tmp,0,16);
				CONVERT.Int32ToDecStrN(tmp, *(s32*)p->addr[i], 0);
				if(addMark)	{
					strcat(str, ", ");
					addMark = 0;
				}
				strcat(str, tmp);
				addMark = 1;
			}
			else if(sym == PACKET_U32){
				memset(tmp,0,16);
				CONVERT.Uint32ToDecStrN(tmp, *(s32*)p->addr[i], 0);
				if(addMark)	{
					strcat(str, ", ");
					addMark = 0;
				}
				strcat(str, tmp);
				addMark = 1;
			}
			else if((sym == PACKET_STR) || (sym == PACKET_PTR)){
				if(addMark)	{
					strcat(str, ", ");
					addMark = 0;
				}
				strcat(str, "\"");
				strcat(str, p->addr[i]);
				strcat(str, "\"");
				addMark = 1;
			}
			else if(sym == PACKET_FLT){
				memset(tmp,0,16);
				//				itoa(*(u32*)p->addr[i],tmp);
				if(addMark)	{
					strcat(str, ", ");
					addMark = 0;
				}
				strcat(str, tmp);
				addMark = 1;
			}
			else if(sym == PACKET_HEX){
				memset(tmp,0,16);
				CONVERT.Uint32ToHexStr(tmp, *(u32*)p->addr[i]);
				if(addMark)	{
					strcat(str, ", ");
					addMark = 0;
				}
				strcat(str, tmp);
				addMark = 1;
			}
			else {	break;	}
			format >>= PACKET_DATA_BW;
		}
		strcat(str, ")");
		strcat(str, (char*)endStr);
		return strlen(str);
	}
	return -100;
}

s16 packetToStrLine(char* str, u8 sz, PAKET_T* p){
	u8 i;
	u32 format;
	char tmp[16];
	u8 addMark = 0, sym;	
	
	if(p == NULL && format==0)	return -1;
	memset(str,0,sz);
	format = p->format[0];
	//line style, 123 abc 

	for(i=0;i<20;i++){ //up to 20 arguments
		if(i==10){	format = p->format[1];}	//reloard
		sym = format & 0x07;
		if(sym == PACKET_S32){
			memset(tmp,0,16);
			CONVERT.Int32ToDecStrN(tmp, *(s32*)p->addr[i], 0);
			if(addMark)	{
				strcat(str, " ");
				addMark = 0;
			}
			strcat(str, tmp);
			addMark = 1;
		}
		else if(sym == PACKET_U32){
			memset(tmp,0,16);
			CONVERT.Uint32ToDecStrN(tmp, *(s32*)p->addr[i], 0);
			if(addMark)	{
				strcat(str, " ");
				addMark = 0;
			}
			strcat(str, tmp);
			addMark = 1;
		}
		else if((sym == PACKET_STR) || (sym == PACKET_PTR)){
			if(addMark)	{
				strcat(str, " ");
				addMark = 0;
			}
			strcat(str, p->addr[i]);
			strcat(str, " ");
			addMark = 1;
		}
		else if(sym == PACKET_FLT){
			memset(tmp,0,16);
			//				itoa(*(u32*)p->addr[i],tmp);
			if(addMark)	{
				strcat(str, " ");
				addMark = 0;
			}
			strcat(str, tmp);
			addMark = 1;
		}
		else if(sym == PACKET_HEX){
			memset(tmp,0,16);
			CONVERT.Uint32ToHexStr(tmp, *(u32*)p->addr[i]);
			if(addMark)	{
				strcat(str, " ");
				addMark = 0;
			}
			strcat(str, tmp);
			addMark = 1;
		}
		else {	break;	}
		format >>= PACKET_DATA_BW;
	}
	strcat(str, " ");
	strcat(str, (char*)endStr);
	return strlen(str);
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

