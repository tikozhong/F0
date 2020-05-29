/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : parammeter.c
* Author             : Tiko Zhong
* Date First Issued  : 05/09/2017
* Description        : This file provides a set of functions needed to manage the
*
*******************************************************************************/
#include "mystring.h"
#include "convert.h"
#include "compiler.h"
//#include "myuart_os.h"
#include "stdarg.h"

/* using external variables ------------------------------------*/
#define COMPLIER_PARAM_LEN	32

/* global variables --------------------------------------------*/
/* private variables --------------------------------------------*/
/* private Functions ---------------------------------------------*/
static s8 intelligentBoardCompile(const char* S, PAKET_T* packet);

s8 Compile(const char* S, ShellType_t type, PAKET_T* packet){
	if(type == SHL_CPP)	return cppStyleCompile(S, packet);
	else if(type == SHL_INTEL_LED)	return intelligentBoardCompile(S, packet);
	else return -1;
}

s8 cppStyleCompile(const char* S, PAKET_T* packet){
	u8 indxD,squ;
	s16 indxS,indx,indxFr,indxTo;
	s8 rtn;
	CONVERT_T convert;
	char tmpStr[64];
	u32 val;
	
	if(S==NULL || packet==NULL)	return -1;
	convertSetup(&convert);
	packetReset(packet);
	packet->style = PAKET_STYLE_CPP;
	
	//test "()"	
	indxFr = indexOf(S, "(");
	indxTo = lastIndxOf(S, ")");
	if( (indxFr <= 0) || (indxTo <= 0) || (indxFr >= indxTo)){	return -1;}
	
	//fetch devname and function name
	memset(tmpStr,0,64);	indxD=0;
	squ = 0;
	for( indxS=0; indxS <= indxFr; indxS++ ){
		switch (squ){
			case 0:
				if(S[indxS]==' ' || S[indxS]=='\t'){
					if(indxD == 0) continue;
					else {	squ = 1; }
				}
				else if(S[indxS]>='0' && S[indxS]<='9' && indxD)	{	tmpStr[indxD++] = S[indxS];}
				else if(S[indxS]>='a' && S[indxS]<='z')						{	tmpStr[indxD++] = S[indxS];}
				else if(S[indxS]>='A' && S[indxS]<='Z')						{	tmpStr[indxD++] = S[indxS]+32;}
				else if(S[indxS]=='_' || S[indxS]=='$')						{	tmpStr[indxD++] = S[indxS];}
				else if(S[indxS]=='.' && indxD){
					packetAddTo(packet, "%s", tmpStr);	//fetched a object name
					packet->option0++;
					memset(tmpStr,0,64);	indxD=0;
				}
				else if(S[indxS]=='(' && indxD){
					packetAddTo(packet, "%s", tmpStr);	//fetched a funtion name
					packet->option0++;
					break;
				}
				else{	return -2;}
				if(indxD >= 64)	{	return -3;}
				break;
				
			case 1:		//expect '.' or '('
				if(S[indxS]==' ' || S[indxS]=='\t')	continue;
				else if(S[indxS]=='.' && indxD){
					packetAddTo(packet, "%s", tmpStr);	//fetched a object name
					packet->option0++;
					memset(tmpStr,0,64);	indxD=0;
					squ = 0;
				}
				else if(S[indxS]=='(' && indxD){
					packetAddTo(packet, "%s", tmpStr);	//fetched a funtion name
					packet->option0++;
					squ = 0;
					break;
				}
				else{	return -2;}
				break;
			default:
				break;
		}
	}
	if(packet->format[0] == 0){	return -3;}

	//fetch params string
	memset(tmpStr,0,64);	indxD=0;
	squ = 0;
	rtn = 0;
	for(indx=indxFr+1; indx<=indxTo; indx++){
		switch(squ){
			case 0:	//wait string or number start
				if(S[indx]==' ' || S[indx]=='\t')	continue;
				else if(S[indx]==')'){
					if(indx == indxTo)	return rtn;
					squ = 90;
				}
				else if(S[indx]=='"'){
					memset(tmpStr,0,64);	indxD=0;
					squ = 1;
				}
				else if((S[indx]>='0' && S[indx]<='9') || S[indx]=='-' || S[indx]=='.')	{
					memset(tmpStr,0,64);	indxD=0;
					tmpStr[indxD++] = S[indx];
					squ = 10;
				}
				else {	return -1;	}
				break;
				
			case 1:	//start to fetch a string
				if(S[indx] == '"')	squ = 2;
				else if(S[indx]>=0x20 && S[indx]<=0x7e)	tmpStr[indxD++] = S[indx];
				else return -4;
				if(indxD>=64)	return -7;
				break;
				
			case 2:	//expect a end symbol ',' or ')'
				if(S[indx]==' ' || S[indx]=='\t')	continue;
				else if(S[indx]==','){	//push a param
					if(strlen((char*)tmpStr)<=0)	return -6;
					packetAddTo(packet, "%s", tmpStr);	//new 
					rtn++;
					squ = 0;
				}
				else if(S[indx]==')'){	//push a param
					if(strlen((char*)tmpStr)<=0)	return -6;
					packetAddTo(packet, "%s", tmpStr);	//new
					if(indx==indxTo)	return rtn;
					rtn++;
					squ = 90;
				}
				else	{ return -5;	}
				break;
				
			case 10:	//number string start
				if((S[indx]>='0' && S[indx]<='9') || S[indx]=='.')	tmpStr[indxD++] = S[indx];
				else if(S[indx] == 'x'){	//keep hex head
					if(indxD==1 && tmpStr[0]=='0'){
						tmpStr[indxD++] = S[indx];
					}
					else	{	return -10;	}
				}
				else if((S[indx]>='a' && S[indx]<='f') || (S[indx]>='A' && S[indx]<='F')){
					if(tmpStr[0]=='0' && tmpStr[1]=='x'){
						tmpStr[indxD++] = S[indx];
					}
					else	{	return -10;	}
				}
				else if(S[indx]==' ' || S[indx]=='\t')	squ = 11;
				else if(S[indx] == ','){
					if(indexOf(tmpStr, ".") >=0 ){	//float
						packetAddTo(packet, "%d", val);	//new 
						memset(tmpStr,0,64);	indxD=0;
						rtn++;	
					}
					else if(tmpStr[0] == '-'){	//int
						if(convert.StrToInt32(tmpStr, (s32*)&val) >= 0){
							packetAddTo(packet, "%d", val);	//new 
							memset(tmpStr,0,64);	indxD=0;
							rtn++;
						}
						else	return -11;
					}
					else{
						if(convert.StrToUint32(tmpStr, &val) < 0)	return -13;
						packetAddTo(packet, "%u", val);	//new 								
						memset(tmpStr,0,64);	indxD=0;
						rtn++;
					}
					squ = 0;
				}
				else if(S[indx]>=')'){
					if(indexOf(tmpStr, ".") >=0 ){		//float
						packetAddTo(packet, "%d", val);	//new 
						memset(tmpStr,0,64);	indxD=0;
						rtn++;	
					}
					else if(tmpStr[0] == '-'){	//int
						if(convert.StrToInt32(tmpStr, (s32*)&val) < 0)	return -12;
						packetAddTo(packet, "%d", val);	//new 
						memset(tmpStr,0,64);	indxD=0;
						rtn++;						
					}
					else{
						if(convert.StrToUint32(tmpStr, &val) < 0)	return -13;
						packetAddTo(packet, "%u", val);	//new 
						memset(tmpStr,0,64);	indxD=0;
						rtn++;	
					}
					if(indx==indxTo)	return rtn;					
					squ = 90;
				}
				else	{ 
					return -6; 
				}
				if(indxD >= 64)	return -7;
				break;
			
			case 11:	//expect a ',' or ')'
				if(S[indx]==' ' || S[indx]=='\t')	break;
				if(S[indx]>=','){
					if(indexOf(tmpStr, ".") >=0 ){	//float
						packetAddTo(packet, "%d", val);	//new 
						memset(tmpStr,0,64);	indxD=0;
						rtn++;	
					}
					else if(tmpStr[0] == '-'){	//int
						if(convert.StrToInt32(tmpStr, (s32*)&val) < 0)	return -12;
						packetAddTo(packet, "%d", val);	//new 
						memset(tmpStr,0,64);	indxD=0;
						rtn++;						
					}
					else{
						if(convert.StrToUint32(tmpStr, &val) < 0)	return -13;
						packetAddTo(packet, "%u", val);	//new 
						memset(tmpStr,0,64);	indxD=0;
						rtn++;	
					}
					squ = 0;
				}
				else if(S[indx]>=')'){
					if(indexOf(tmpStr, ".") >=0 ){	//float
						packetAddTo(packet, "%d", val);	//new 
						memset(tmpStr,0,64);	indxD=0;
						rtn++;	
					}
					else if(tmpStr[0] == '-'){	//int
						if(convert.StrToInt32(tmpStr, (s32*)&val) < 0)	return -12;
						packetAddTo(packet, "%d", val);	//new 
						memset(tmpStr,0,64);	indxD=0;
						rtn++;						
					}
					else{
						if(convert.StrToUint32(tmpStr, &val) < 0)	return -13;
						packetAddTo(packet, "%u", val);	//new 
						memset(tmpStr,0,64);	indxD=0;
						rtn++;	
					}
					if(indx==indxTo)	return rtn;					
					squ = 90;
				}
				else	return -11;
				break;
				
			case 90:	//expect end
				if(indx==indxTo)	return rtn;
				else if(S[indx]==' ' || S[indx]=='\t' || S[indx]=='\r' || S[indx]=='\n')	continue;
				else {return -20;}
			
			default:
				return -100;
		}
	}
	return rtn;
}

static s8 intelligentBoardCompile(const char* S, PAKET_T* packet){
	u8 indxD,squ;
	s16 indxS,indx,indxFr,indxTo;
	s8 rtn;
	CONVERT_T CONVERT;
	char tmpStr[64];
	u32 val;
	
	
	if(S==NULL || packet==NULL)	return -1;	
	convertSetup(&CONVERT);
	packetReset(packet);	
	
	//test "()"	
	indxFr = indexOf(S, "(");
	indxTo = lastIndxOf(S, ")");
	if( (indxFr <= 0) || (indxTo <= 0) || (indxFr >= indxTo)){	return -1;}
	
	//fetch devname and function name
	memset(tmpStr,0,64);	indxD=0;
	squ = 0;
	for( indxS=0; indxS <= indxFr; indxS++ ){
		switch (squ){
			case 0:
				if(S[indxS]==' ' || S[indxS]=='\t'){
					if(indxD == 0) continue;
					else {	squ = 1; }
				}
				else if(S[indxS]>='0' && S[indxS]<='9' && indxD)	{	tmpStr[indxD++] = S[indxS];}
				else if(S[indxS]>='a' && S[indxS]<='z')						{	tmpStr[indxD++] = S[indxS];}
				else if(S[indxS]>='A' && S[indxS]<='Z')						{	tmpStr[indxD++] = S[indxS]+32;}
				else if(S[indxS]=='_' || S[indxS]=='$')						{	tmpStr[indxD++] = S[indxS];}
				else if(S[indxS]=='.' && indxD){
					packetAddTo(packet, "%s", tmpStr);	//fetched a object name
					packet->option0++;
					memset(tmpStr,0,64);	indxD=0;
				}
				else if(S[indxS]=='(' && indxD){
					packetAddTo(packet, "%s", tmpStr);	//fetched a funtion name
					packet->option0++;
					break;
				}
				else{	return -2;}
				if(indxD >= 64)	{	return -3;}
				break;
				
			case 1:		//expect '.' or '('
				if(S[indxS]==' ' || S[indxS]=='\t')	continue;
				else if(S[indxS]=='.' && indxD){
					packetAddTo(packet, "%s", tmpStr);	//fetched a object name
					packet->option0++;
					memset(tmpStr,0,64);	indxD=0;
					squ = 0;
				}
				else if(S[indxS]=='(' && indxD){
					packetAddTo(packet, "%s", tmpStr);	//fetched a funtion name
					packet->option0++;
					squ = 0;
					break;
				}
				else{	return -2;}
				break;
			default:
				break;
		}
	}
	if(packet->format[0] == 0){	return -3;}

	//fetch params string
	memset(tmpStr,0,64);	indxD=0;
	squ = 0;
	rtn = 0;
	for(indx=indxFr+1; indx<=indxTo; indx++){
		switch(squ){
			case 0:	//wait string or number start
				if(S[indx]==' ' || S[indx]=='\t')	continue;
				else if(S[indx]==')'){
					if(indx == indxTo)	return rtn;
					squ = 90;
				}
				else if(S[indx]=='"'){
					memset(tmpStr,0,64);	indxD=0;
					squ = 1;
				}
				else if((S[indx]>='0' && S[indx]<='9') || S[indx]=='-' || S[indx]=='.')	{
					memset(tmpStr,0,64);	indxD=0;
					tmpStr[indxD++] = S[indx];
					squ = 10;
				}
				else {	return -1;	}
				break;
				
			case 1:	//start to fetch a string
				if(S[indx] == '"')	squ = 2;
				else if(S[indx]>=0x20 && S[indx]<=0x7e)	tmpStr[indxD++] = S[indx];
				else return -4;
				if(indxD>=64)	return -7;
				break;
				
			case 2:	//expect a end symbol ',' or ')'
				if(S[indx]==' ' || S[indx]=='\t')	continue;
				else if(S[indx]==','){	//push a param
					if(strlen((char*)tmpStr)<=0)	return -6;
					packetAddTo(packet, "%s", tmpStr);	//new 
					rtn++;
					squ = 0;
				}
				else if(S[indx]==')'){	//push a param
					if(strlen((char*)tmpStr)<=0)	return -6;
					packetAddTo(packet, "%s", tmpStr);	//new
					if(indx==indxTo)	return rtn;
					rtn++;
					squ = 90;
				}
				else	{ return -5;	}
				break;
				
			case 10:	//number string start
				if((S[indx]>='0' && S[indx]<='9') || S[indx]=='.')	tmpStr[indxD++] = S[indx];
				else if((S[indx]>='a' && S[indx]<='f') || (S[indx]>='A' && S[indx]<='F'))	tmpStr[indxD++] = S[indx];
				else if(S[indx]==' ' || S[indx]=='\t')	squ = 11;
				else if(S[indx]>=','){
					if(indexOf(tmpStr, ".") >=0 ){	//float
						packetAddTo(packet, "%d", val);	//new 
						memset(tmpStr,0,64);	indxD=0;
						rtn++;	
					}
					else if(tmpStr[0] == '-'){	//int
						if(CONVERT.StrToInt32(tmpStr, (s32*)&val) >= 0){
							packetAddTo(packet, "%d", val);	//new 
							memset(tmpStr,0,64);	indxD=0;
							rtn++;
						}
						else	return -11;
					}
					else{
						if(CONVERT.StrToUint32(tmpStr, &val) < 0)	return -13;
						packetAddTo(packet, "%u", val);	//new 
						memset(tmpStr,0,64);	indxD=0;
						rtn++;	
					}
					squ = 0;
				}
				else if(S[indx]>=')'){
					if(indexOf(tmpStr, ".") >=0 ){	//float
						packetAddTo(packet, "%d", val);	//new 
						memset(tmpStr,0,64);	indxD=0;
						rtn++;	
					}
					else if(tmpStr[0] == '-'){	//int
						if(CONVERT.StrToInt32(tmpStr, (s32*)&val) < 0)	return -12;
						packetAddTo(packet, "%d", val);	//new 
						memset(tmpStr,0,64);	indxD=0;
						rtn++;						
					}
					else{
						if(CONVERT.StrToUint32(tmpStr, &val) < 0)	return -13;
						packetAddTo(packet, "%u", val);	//new 
						memset(tmpStr,0,64);	indxD=0;
						rtn++;	
					}
					if(indx==indxTo)	return rtn;					
					squ = 90;
				}
				else	{ return -6; }
				if(indxD>=64)	return -7;
				break;
			
			case 11:	//expect a ',' or ')'
				if(S[indx]==' ' || S[indx]=='\t')	break;
				if(S[indx]>=','){
					if(indexOf(tmpStr, ".") >=0 ){	//float
						packetAddTo(packet, "%d", val);	//new 
						memset(tmpStr,0,64);	indxD=0;
						rtn++;	
					}
					else if(tmpStr[0] == '-'){	//int
						if(CONVERT.StrToInt32(tmpStr, (s32*)&val) < 0)	return -12;
						packetAddTo(packet, "%d", val);	//new 
						memset(tmpStr,0,64);	indxD=0;
						rtn++;						
					}
					else{
						if(CONVERT.StrToUint32(tmpStr, &val) < 0)	return -13;
						packetAddTo(packet, "%u", val);	//new 
						memset(tmpStr,0,64);	indxD=0;
						rtn++;	
					}
					squ = 0;
				}
				else if(S[indx]>=')'){
					if(indexOf(tmpStr, ".") >=0 ){	//float
						packetAddTo(packet, "%d", val);	//new 
						memset(tmpStr,0,64);	indxD=0;
						rtn++;	
					}
					else if(tmpStr[0] == '-'){	//int
						if(CONVERT.StrToInt32(tmpStr, (s32*)&val) < 0)	return -12;
						packetAddTo(packet, "%d", val);	//new 
						memset(tmpStr,0,64);	indxD=0;
						rtn++;						
					}
					else{
						if(CONVERT.StrToUint32(tmpStr, &val) < 0)	return -13;
						packetAddTo(packet, "%u", val);	//new 
						memset(tmpStr,0,64);	indxD=0;
						rtn++;	
					}
					if(indx==indxTo)	return rtn;					
					squ = 90;
				}
				else	return -11;
				break;
				
			case 90:	//expect end
				if(indx==indxTo)	return rtn;
				else if(S[indx]==' ' || S[indx]=='\t' || S[indx]=='\r' || S[indx]=='\n')	continue;
				else {return -20;}
			
			default:
				return -100;
		}
	}
	return rtn;
}


u8 cppStyleIsMatchDev(PAKET_T* packet, const char* DevName){
	if(isSameStr(packet->addr[0], DevName)){
		return 1;
	}
	return 0;
}

u8 cppStyleIsMatchFunc(PAKET_T* packet, const char* FuncName){
	if(isSameStr(packet->addr[1], FuncName)){
		return 1;
	}
	return 0;
}

u8 cppStyleIsMatchArgu(PAKET_T* packet, const char* Format){
	char str[16] = "%s%s";
	if(Format)	strcat(str, Format);
	return (packetIsMatch(packet, str));
}

/*******************************************************************************
* Function Name  : split
* Description    : 
* Input          : pRsrc
								 : 
* Output         : None
* Return         : <0			-can not match function name
								 : >=0		-the count of params
*******************************************************************************/
s8 split(PAKET_T* packet, const char* LINE){

	s8 rtn;
	CONVERT_T CONVERT;
	char tmpStr[64];
	u8 squ,markCount;
	u16 indx,indxD;
	u32 tmpU32;
	s32 tmpS32;
	const char* POS = LINE;
	
	if((LINE == NULL) || (packet == NULL))	return -1;	
	convertSetup(&CONVERT);
	packetReset(packet);
	packetSetStyle(packet, PAKET_STYLE_ORG);
//	for(indx=0; LINE[indx] != 0; indx++){
//		if(LINE[indx] == ' ' || LINE[indx] == '\t')	continue;
//		else POS = &LINE[indx];
//	}
	//fetch params string
//	memset(tmpStr,0,64);	indxD=0;
	squ = 0;
	rtn = 0;
	markCount = 1;	//do not allow bigin with ','
	//cldr_t,carrier,00,01,02,03
	for(indx=0; POS[indx] != 0; indx++){
		switch(squ){
			case 0:	//wait string without begin with SYMBOL1 or SYMBOL2
				if(POS[indx]>=0x20 && POS[indx]<=0x7e){
					if(POS[indx] == ',' ){
						if(markCount > 0)	return -2;
						else break;
					}						
					else if(POS[indx] == ' ')	break;
					else if(POS[indx] == ';')	{
						if(markCount > 0)	return -2;
						else break;
					}
					else if(indexOf(&POS[indx], "0x")==0){
						memset(tmpStr,0,64);	indxD=0;
						tmpStr[indxD++] = '0';
						tmpStr[indxD++] = 'x';
						indx++;
						squ = 2;	
					}
					else if(POS[indx] == '-' || (POS[indx] >= '0' && POS[indx] <= '9')){
						memset(tmpStr,0,64);	indxD=0;
						tmpStr[indxD++] = POS[indx];
						squ = 3;
					}
					else {
						memset(tmpStr,0,64);	indxD=0;
						tmpStr[indxD++] = POS[indx];
						squ = 1;
					}
				}
				else	return -3;
				break;
				
			case 1:	//string
				if(POS[indx]>=0x20 && POS[indx]<=0x7e){
					if(POS[indx] == ',' || POS[indx] == ' ' || POS[indx]==';'){	//add to packet
						packetAddTo(packet, "%s", tmpStr);	//new
						rtn++;
						if(POS[indx] == ',' || POS[indx] == ' ')	markCount = 1;
						else markCount = 0;
						if(POS[indx]==';')	return rtn;
						else 	squ = 0;
					}
					else	tmpStr[indxD++] = POS[indx];
				}
				else	return -4;
				break;
				
			case 2:	//hex number	
				if(POS[indx] == ',' || POS[indx] == ' ' || POS[indx]==';'){	//add to packet
					CONVERT.StrToUint32(tmpStr, &tmpU32);
					packetAddTo(packet, "%u", tmpU32);	//new
					rtn++;
					if(POS[indx] == ',' || POS[indx] == ' ')	markCount = 1;
					else markCount = 0;
					if(POS[indx]==';')	return rtn;
					else 	squ = 0;
				}
				else if((POS[indx]>='0' && POS[indx]<='9') || (POS[indx]>='a' && POS[indx]<='f'))
					tmpStr[indxD++] = POS[indx];
				else	return -5;		
				break;
				
			case 3:	//decimal number	
				if(POS[indx] == ',' || POS[indx] == ' ' || POS[indx]==';'){	//add to packet
					CONVERT.StrToInt32(tmpStr, &tmpS32);
					packetAddTo(packet, "%d", tmpS32);	//new
					rtn++;
					if(POS[indx] == ',' || POS[indx] == ' ')	markCount = 1;
					else markCount = 0;
					if(POS[indx]==';')	return rtn;
					else 	squ = 0;
				}
				else if(POS[indx]>='0' && POS[indx]<='9')	tmpStr[indxD++] = POS[indx];
				else	return -6;
				break;
			default:
				return -100;
		}
	}
	//push last
	switch (squ){
		case 0:
			if(markCount>0)	return -101;
		case 1:
			packetAddTo(packet, "%s", tmpStr);	//new
			rtn++;
			break;
		case 2:
			CONVERT.StrToUint32(tmpStr, &tmpU32);
			packetAddTo(packet, "%u", tmpU32);	//new
			rtn++;
			break;
		case 3:
			CONVERT.StrToInt32(tmpStr, &tmpS32);
			packetAddTo(packet, "%d", tmpS32);	//new
			rtn++;
			break;
		default:
			break;
	}
	
	return rtn;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

