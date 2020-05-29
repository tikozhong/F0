/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : convert.c
* Author             : Tiko Zhong
* Date First Issued  : 12/01/2015
* Description        : This file provides a set of functions needed to manage the
*
*******************************************************************************/
#include "string.h"
#include "convert.h"

/* using external variables ------------------------------------*/
/* global variables --------------------------------------------*/
/* private variables --------------------------------------------*/
/* private Functions ---------------------------------------------*/
static void uint32ToDecStrN(char* s, u32 num, u8 nDigi);
static void int32ToDecStrN(char* s, s32 num, u8 nDigi);
static void uint32ToHexStr(char* s, u32 num);
static void uint16ToHexStr(char* s, u16 num);
static void uint8ToHexStr(char* s, u8 num);
static void uint32ToBinStr(char* s, u32 num);
static void floatToStr(char* s, s32 mul, u32 div);

static void strCatDecN_Uint32(char* s, u32 num, u8 nDigi);
static void strCatDecN_Int32(char* s, s32 num, u8 nDigi);
static void strCatHex_Uint32(char* s, u32 num);
static void strCatHex_Uint16(char* s, u16 num);
static void strCatHex_Uint8(char* s, u8 num);
static void strCatBin_Uint32(char* s, u32 num);
static void strCat_Float(char* s, s32 Mul, u32 Div);

static s8 strToUint32(const char* S, u32 *val);
static s8 strToInt32(const char* S, s32 *val);
static s8 strToFloat(const char *S, s32 *Mul, u32 *Div);

static void uint32ToU8Array(u32, u8*);
static void uint16ToU8Array(u16, u8*);
static u32 u8ArrayToUint32(u8*);
static u16 u8ArrayToUint16(u8*);

void convertSetup(CONVERT_T *pDev){
	pDev->Uint32ToDecStrN = uint32ToDecStrN;	//	s8 (*Uint32ToDecStrN)(u8* s, u32 num, u8 nDigi);
	pDev->Int32ToDecStrN = int32ToDecStrN;		//	s8 (*Int32ToDecStrN)(u8* s, s32 num, u8 nDigi);
	pDev->Uint32ToHexStr = uint32ToHexStr;		//	s8 (*Uint32ToHexStr)(u8* s, u32 num);
	pDev->Uint16ToHexStr = uint16ToHexStr;		//	s8 (*Uint16ToHexStr)(u8* s, u16 num);
	pDev->Uint8ToHexStr = uint8ToHexStr;			//	s8 (*Uint8ToHexStr)(u8* s, u8 num);
	pDev->Uint32ToBinStr = uint32ToBinStr;		//	s8 (*Uint32ToBinStr)(u8* s, u32 num);
	pDev->FloatToStr = floatToStr;
	
	pDev->Uint32ToStrCatDecN = strCatDecN_Uint32;	//	s8 (*Uint32ToStrCatDecN)(u8* s, u32 num, u8 nDigi);
	pDev->Int32ToStrCatDecN = strCatDecN_Int32;		//	s8 (*Int32ToStrCatDecN)(u8* s, s32 num, u8 nDigi);
	pDev->Uint32ToStrCatHex = strCatHex_Uint32;		//	s8 (*Uint32ToStrCatHex)(u8* s, u32 num);
	pDev->Uint16ToStrCatHex = strCatHex_Uint16;		//	s8 (*Uint16ToStrCatHex)(u8* s, u16 num);
	pDev->Uint8ToStrCatHex = strCatHex_Uint8;			//	s8 (*Uint8ToStrCatHex)(u8* s, u8 num);
	pDev->Uint32ToStrCatBin = strCatBin_Uint32;		//	s8 (*Uint32ToStrCatBin)(u8* s, u32 num);
	pDev->FloatToStrCat = strCat_Float;
	
	pDev->StrToUint32 = strToUint32;	//	s8 (*StrToUint32)(const u8* S, u32 *val);
	pDev->StrToInt32 = strToInt32;		//	s8 (*StrToInt32)(const u8* S, s32 *val);
	pDev->StrToFloat = strToFloat;		//	s8 (*StrToFloat)(const u8*S, s32* mul , u32* div);

	pDev->Uint32ToU8Array = uint32ToU8Array;
	pDev->Uint16ToU8Array = uint16ToU8Array;
	pDev->U8ArrayToUint32 = u8ArrayToUint32;
	pDev->U8ArrayToUint16 = u8ArrayToUint16;
}

static void uint32ToDecStrN(char* s, u32 num, u8 nDigi){
	u8 i,j,k,digital,zeroEn,strTmp[20]={0};
	u32 tmp0, tmp1, base;
	
	if(s == NULL)	return;
	if(nDigi >= 10)	digital = 10;
	else digital = nDigi;
	
	tmp0 = num;
	zeroEn = 0;
	base = 1000000000;
	for(i=0;1;){
		tmp1 = tmp0/base;
		if(tmp1){	strTmp[i++]=tmp1+'0'; zeroEn=1; }
		if(tmp1==0 && zeroEn)	strTmp[i++] = '0';
		tmp0 = tmp0%base;	base /= 10;
		if(base == 1){ strTmp[i++]=tmp0+'0'; break; }
	}
	//copy
	j = strlen((const char*)strTmp);
	if(digital == 0){ for(i=0;i<j;i++) s[i] = strTmp[i]; }
	else{
		for(i=0,k=0;i<digital;i++){
			if( i+j < digital )	s[i] = '0';
			else	{	s[i] = strTmp[k];	k++;	}
		}		
	}
}

static void int32ToDecStrN(char* s, s32 num, u8 nDigi){
	u32 tmp = (num^0xffffffff)+1;
	if(num < 0){
		s[0] = '-';
		uint32ToDecStrN(&s[1], tmp, nDigi);
	}
	else{ uint32ToDecStrN(s, num, nDigi); }
}

static void uint32ToHexStr(char* s, u32 num){
	u8 i = 0;
	u32 tmp0;

	tmp0 = num;
	s[9] = tmp0 & 0x0f;    tmp0 >>= 4;
	s[8] = tmp0 & 0x0f;    tmp0 >>= 4;
	s[7] = tmp0 & 0x0f;    tmp0 >>= 4;
	s[6] = tmp0 & 0x0f;    tmp0 >>= 4;
	s[5] = tmp0 & 0x0f;    tmp0 >>= 4;
	s[4] = tmp0 & 0x0f;    tmp0 >>= 4;
	s[3] = tmp0 & 0x0f;    tmp0 >>= 4;
	s[2] = tmp0 & 0x0f;
	s[1] = 'x';
	s[0] = '0';

	for (i = 2; i <= 9; i++ ){
		if(s[i] >= 10 )	s[i] = s[i] - 10 + 'A';
		else	s[i] += '0';
	}
}

static void uint16ToHexStr(char* s, u16 num){
	u8 i = 0;
	u16 tmp0;

	tmp0 = num;
	s[5] = tmp0 & 0x0f;    tmp0 >>= 4;
	s[4] = tmp0 & 0x0f;    tmp0 >>= 4;
	s[3] = tmp0 & 0x0f;    tmp0 >>= 4;
	s[2] = tmp0 & 0x0f;
	s[1] = 'x';
	s[0] = '0';

	for (i = 2; i <= 5; i++ ){
		if(s[i] >= 10 )	s[i] = s[i] - 10 + 'A';
		else	s[i] += '0';
	}
}

static void uint8ToHexStr(char* s, u8 num){
	u8 i = 0;
	u8 tmp0;

	tmp0 = num;
	s[3] = tmp0 & 0x0f;    tmp0 >>= 4;
	s[2] = tmp0 & 0x0f;
	s[1] = 'x';
	s[0] = '0';

	for (i = 2; i <= 3; i++ ){
		if(s[i] >= 10 )	s[i] = s[i] - 10 + 'A';
		else	s[i] += '0';
	}
}

static void uint32ToBinStr(char* s, u32 num){
	u32 tmp0;
	u8 i, j;

	s[0] = '0';	s[1] = 'b';
	tmp0 = num;
	for ( i = 2, j = 0; i < 34 + 7; i++ ){
		if ( tmp0 & 0x80000000 )	s[i] = '1';
		else	s[i] = '0';
		tmp0 <<= 1; j++;
		if ( j >= 4 ){	j = 0;	s[++i] = ' ';	}
	}
}

static void floatToStr(char* s, s32 Mul, u32 Div){
	u8 indx;
	u32 mul, div, a,b,x;

	if((Div == 0) || (s == NULL))	return;
	indx = 0;
	if(Mul<0){
		mul = (Mul^0xffffffff)+1;
		s[0] = '-';
	}
	else	mul = Mul;
	div = Div;
	a = mul/div;
	b = mul%div;

	for(x=1;1;){	
		if(x*10 <= (0xffffffff/Div))	x *= 10;
		else break;
	}
	b *= x;	b /= div;
	//
	indx = strlen((const char*)s);
	uint32ToDecStrN(&s[indx], a, 0);
	indx = strlen((const char*)s);
	s[indx++] = '.';
	uint32ToDecStrN(&s[indx],b,0);
}

static void strCatDecN_Uint32(char* s, u32 num, u8 nDigi){
	u16 i;	
	i = strlen((const char*)s);
	uint32ToDecStrN(&s[i], num, nDigi);
}

static void strCatDecN_Int32(char* s, s32 num, u8 nDigi){
	u16 i;	
	i = strlen((const char*)s);
	int32ToDecStrN(&s[i], num, nDigi);
}

static void strCatHex_Uint32(char* s, u32 num){
	u16 i;	
	i = strlen((const char*)s);
	uint32ToHexStr(&s[i], num);
}

static void strCatHex_Uint16(char* s, u16 num){
	u16 i;	
	i = strlen((const char*)s);
	uint16ToHexStr(&s[i], num);
}

static void strCatHex_Uint8(char* s, u8 num){
	u16 i;	
	i = strlen((const char*)s);
	uint8ToHexStr(&s[i], num);
}

static void strCatBin_Uint32(char* s, u32 num){
	u16 i;	
	i = strlen((const char*)s);
	uint32ToBinStr(&s[i], num);
}

static void strCat_Float(char* s, s32 Mul, u32 Div){
	u16 i;	
	i = strlen((const char*)s);
	floatToStr(&s[i], Mul, Div);
}

//HEX MUST BE STARTED WITH "0X" OR "0x"
static s8 strToUint32(const char* S, u32 *val){
	u8 i, j, num[10];
	u32 base;

	memset(num, 0 , 10);
	for ( i=0,j=0; 1; i++)
	{
		if( S[i] == 0 )	break;
		if(j){
			if (	( S[i]>='0' && S[i]<='9' ) ||(S[i]=='x')||(S[i]=='X')
				||(S[i]=='a')||(S[i]=='b')||(S[i]=='c')||(S[i]=='d')||(S[i]=='e')||(S[i]=='f')
				||(S[i]=='A')||(S[i]=='B')||(S[i]=='C')||(S[i]=='D')||(S[i]=='E')||(S[i]=='F')	) {
				if(j >= 10)	return -4;
				num[9-j] = S[i];
				j ++;
			}
			else if( S[i]==' ' || S[i]=='\t' || S[i]=='\r' || S[i]=='\n' || S[i]==',' )	break;
			else return -2;
		}
		else{
			if ( S[i] >= '0' && S[i] <= '9' ){	num[9-j] = S[i];	j++;	}
			else if(S[i]==' ' || S[i]=='\t' || S[i]=='\r' || S[i]=='\n')	continue;
			else return -1;
		}
	}
	//j 保存的是有效字符的数量
	if(j==0 || num[9]==0)	return -3;
	//Hex or Dec
	if(num[9]=='0' && num[8]=='x' && j>=3){
		//compute
		*val = 0;
		for ( i = 7; 1; i-- )
		{
			if( num[i] == 0 )	break;
			switch ( num[i] )
			{
				case '0':	*val <<= 4;			break;
				case '1':	*val <<= 4;	*val |= 0x01;		break;
				case '2':	*val <<= 4;	*val |= 0x02;		break;
				case '3':	*val <<= 4;	*val |= 0x03;		break;
				case '4': *val <<= 4;	*val |= 0x04;		break;
				case '5':	*val <<= 4;	*val |= 0x05;		break;
				case '6':	*val <<= 4;	*val |= 0x06;		break;
				case '7':	*val <<= 4;	*val |= 0x07;		break;
				case '8':	*val <<= 4;	*val |= 0x08;		break;
				case '9':	*val <<= 4;	*val |= 0x09; 	break;
				case 'A':	case 'a':	*val <<= 4;	*val |= 0x0a;		break;
				case 'B':	case 'b':	*val <<= 4;	*val |= 0x0b; 	break;
				case 'C':	case 'c':	*val <<= 4;	*val |= 0x0c; 	break;
				case 'D':	case 'd':	*val <<= 4;	*val |= 0x0d;		break;
				case 'E':	case 'e':	*val <<= 4;	*val |= 0x0e;		break;
				case 'F':	case 'f': *val <<= 4; *val |= 0x0f; 	break;
				default: 	return -8;	
			}
			if(i==0)	break;
		}
	}
	else{
		//compute
		*val = 0;	base = 1;
		for ( i = 0; i < 10; i++ )
		{
			if(num[i]==0)	continue;
			if ( num[i] >= '0' && num[i] <= '9' )
			{	*val += ( num[i] - '0' ) * base;	base *= 10;}
			else return -6;
		}
	}
	return j;
}

static s8 strToInt32(const char* S, s32 *val){
	u8 i, j, num[10],isNeg = 0;
	u32 base;

	memset(num, 0 , 10);
	for ( i=0,j=0; 1; i++)
	{
		if( S[i] == 0 )	break;
		if(j){
			if (	( S[i]>='0' && S[i]<='9' ) ||(S[i]=='x')||(S[i]=='X')
				||(S[i]=='a')||(S[i]=='b')||(S[i]=='c')||(S[i]=='d')||(S[i]=='e')||(S[i]=='f')
				||(S[i]=='A')||(S[i]=='B')||(S[i]=='C')||(S[i]=='D')||(S[i]=='E')||(S[i]=='F')	) {
				if(j >= 10)	return -4;
				num[9-j] = S[i];
				j ++;
			}
			else if( S[i]==' ' || S[i]=='\t' || S[i]=='\r' || S[i]=='\n' || S[i]==',')	break;
			else return -2;
		}
		else{
			if ( S[i] >= '0' && S[i] <= '9' ){	num[9-j] = S[i];	j++;	}
			else if(S[i] == '-'){ isNeg = 1;	}
			else if(S[i]==' ' || S[i]=='\t' || S[i]=='\r' || S[i]=='\n')	continue;
			else return -1;
		}
	}
	//j 保存的是有效字符的数量
	if(j==0 || num[9]==0)	return -3;
	//Hex or Dec
	if(num[9]=='0' && num[8]=='x' && j>=3){
		if(isNeg)	return -10;
		//compute
		*val = 0;
		for ( i = 7; 1; i-- )
		{
			if( num[i] == 0 )	break;
			switch ( num[i] )
			{
				case '0':	*val <<= 4;			break;
				case '1':	*val <<= 4;	*val |= 0x01;		break;
				case '2':	*val <<= 4;	*val |= 0x02;		break;
				case '3':	*val <<= 4;	*val |= 0x03;		break;
				case '4': *val <<= 4;	*val |= 0x04;		break;
				case '5':	*val <<= 4;	*val |= 0x05;		break;
				case '6':	*val <<= 4;	*val |= 0x06;		break;
				case '7':	*val <<= 4;	*val |= 0x07;		break;
				case '8':	*val <<= 4;	*val |= 0x08;		break;
				case '9':	*val <<= 4;	*val |= 0x09; 	break;
				case 'A':	case 'a':	*val <<= 4;	*val |= 0x0a;		break;
				case 'B':	case 'b':	*val <<= 4;	*val |= 0x0b; 	break;
				case 'C':	case 'c':	*val <<= 4;	*val |= 0x0c; 	break;
				case 'D':	case 'd':	*val <<= 4;	*val |= 0x0d;		break;
				case 'E':	case 'e':	*val <<= 4;	*val |= 0x0e;		break;
				case 'F':	case 'f': *val <<= 4; *val |= 0x0f; 	break;
				default: 	return -8;	
			}
			if(i==0)	break;
		}
	}
	else{
		//compute
		*val = 0;	base = 1;
		for ( i = 0; i < 10; i++ )
		{
			if(num[i]==0)	continue;
			if ( num[i] >= '0' && num[i] <= '9' )
			{	*val += ( num[i] - '0' ) * base;	base *= 10;}
			else return -6;
		}
		if(isNeg)	*val = 0 - *val;
	}
	return j;
}

static s8 strToFloat(const char *S, s32 *Mul, u32 *Div){
	u8 i, j, k, isNeg = 0, mulStage=0, divStage=0, mulIndx=0, divIndx=0;
	u32 base,div,mul;
	char num[10], divNum[10];

	memset(num, 0 , 10);
	memset(divNum, 0 , 10);
	for ( i=0,j=0; 1; i++)
	{
		if( S[i] == 0 )	break;
		if(mulStage){
			if ( S[i]>='0' && S[i]<='9' ) {
				if(mulIndx > 9)	return -6;
				num[mulIndx] = S[i];
				j ++;	mulIndx++;
			}
			else if(S[i]=='.'){ divStage = 1; mulStage = 0; j++;	}
			else if( S[i]==' ' || S[i]=='\t' || S[i]=='\r' || S[i]=='\n' || S[i]==',')	break;
			else return -2;
		}
		else if(divStage){
			if ( S[i]>='0' && S[i]<='9' ) {
				if(divIndx > 9)	return -5;
				divNum[divIndx] = S[i];
				j ++;	divIndx++;
			}
			else if( S[i]==' ' || S[i]=='\t' || S[i]=='\r' || S[i]=='\n' || S[i]==',')	break;
			else return -2;
		}
		else{
			if ( S[i] >= '0' && S[i] <= '9'){
				num[mulIndx] = S[i];	j++;	mulIndx++; mulStage = 1; }
			else if (S[i]=='-'){	j++;	 mulStage = 1;	isNeg = 1;	}
			else if(S[i]=='.'){	j++; divStage = 1;}
			else if(S[i]==' ' || S[i]=='\t' || S[i]=='\r' || S[i]=='\n')	continue;
			else return -1;
		}
	}
	//compute	
	k = strlen((const char*)divNum);
	for(i=0,base=1; i<k; i++)	base *= 10;
	if(strToUint32(num, &mul) <0 ) return -3;
	if(strToUint32(divNum, &div) <0 ) return -4;
	if(isNeg)	*Mul = 0-(mul*base + div);
	else	*Mul = mul*base + div;
	*Div = base;
	return j;
}

static void uint32ToU8Array(u32 val, u8* pDat){
	u32 tmp = val;
	pDat[0] = tmp & 0xff;	tmp >>= 8;
	pDat[1] = tmp & 0xff;	tmp >>= 8;
	pDat[2] = tmp & 0xff;	tmp >>= 8;
	pDat[3] = tmp & 0xff;
}

static void uint16ToU8Array(u16 val, u8* pDat){
	u16 tmp = val;
	pDat[0] = tmp & 0xff;	tmp >>= 8;
	pDat[1] = tmp & 0xff;
}

static u32 u8ArrayToUint32(u8* pDat){
	u32 tmp;
	tmp = pDat[3];	tmp <<= 8;
	tmp |= pDat[2];	tmp <<= 8;
	tmp |= pDat[1];	tmp <<= 8;
	tmp |= pDat[0];	
	return tmp;
}

static u16 u8ArrayToUint16(u8* pDat){
	u16 tmp;
	tmp = pDat[1];	tmp <<= 8;
	tmp |= pDat[0];	
	return tmp;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
