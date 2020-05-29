/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : parammeter.c
* Author             : Tiko Zhong
* Date First Issued  : 05/09/2017
* Description        : This file provides a set of functions needed to manage the
*
*******************************************************************************/
#include "mystring.h"
#include "convert.h"
#include "parammeter.h"

/* using external variables ------------------------------------*/
/* global variables --------------------------------------------*/
/* private variables --------------------------------------------*/
#define PARAM_LEN 128
#define FUNNAME_LEN 64

/* private Functions ---------------------------------------------*/
//parameters
static void paramReset(PARAM_RSRC_T *pRsrc);
static void paramAddNum(PARAM_RSRC_T *pRsrc, u32 num);
static void paramAddStr(PARAM_RSRC_T *pRsrc, const u8 *str, u16 len);
//static s8 fetchFun(const u8* STR, PARAM_DEV_T *e, CMD_DEF_T* DEF, u8 defLen);
static s8 meetStr(PARAM_RSRC_T *pRsrc, const u8* S);
static s8 meetNum(PARAM_RSRC_T *pRsrc, const u8* S);

static s8 comply(PARAM_RSRC_T* pRsrc, const u8* STR, CMD_DEF_T* DEF, u8 defLen);

void setupComplier(COMPLIER_DEV_T *pDev){
	pDev->Comply = comply;
	paramReset(&pDev->rsrc);
}

static void paramReset(PARAM_RSRC_T *pRsrc){
	u8 i;
	pRsrc->indx = 0;
	pRsrc->indxStr = 0;
	pRsrc->indxU32 = 0;
	pRsrc->format = 0;	
	memset(pRsrc->funName, 0, FUN_NAME_SZ);
	for(i=0;i<PARAM_SUM;i++){
		pRsrc->params[i] = NULL;
		pRsrc->paramU32[i] = 0;
		memset(pRsrc->paramStr[i], 0, PARAM_STR_SZ);
	}
}

static void paramAddNum(PARAM_RSRC_T *pRsrc, u32 num){
	pRsrc->paramU32[pRsrc->indxU32] = num;
	pRsrc->params[pRsrc->indx] = &pRsrc->paramU32[pRsrc->indxU32];
	pRsrc->indxU32++;
	pRsrc->indx++;
}

static void paramAddStr(PARAM_RSRC_T *pRsrc, const u8 *str, u16 len){
	u8 i;
	CONVERT_T CONVERT;
	convertSetup(&CONVERT);
	for(i=0;i<len;i++){
		if(str[i] == 0)	break;
		pRsrc->paramStr[pRsrc->indxStr][i] = str[i];
	}
	pRsrc->params[pRsrc->indx] = pRsrc->paramStr[pRsrc->indxStr];
	pRsrc->indxStr++;
	pRsrc->format |= (u32)1<<pRsrc->indx;
	pRsrc->indx++;
}

static s8 meetNum(PARAM_RSRC_T *pRsrc, const u8* S){
	u32 tmp0;
	s32 tmp1;
	CONVERT_T CONVERT;
	convertSetup(&CONVERT);
	if(CONVERT.StrToUint32(S, &tmp0) >= 0){
		paramAddNum(pRsrc, tmp0);
		return 1;
	}
	else if(CONVERT.StrToInt32(S, &tmp1) >= 0){
		paramAddNum(pRsrc, tmp1);
		return 2;
	}
	else return -1;
}

static s8 meetStr(PARAM_RSRC_T *pRsrc, const u8* S){
	s16 i,j;
	
	i = indexOf(S, (const u8*)"\"");
	j = lastIndxOf(S, (const u8*)"\"");
	
	if(i<j && i>=0 && j>=0){
		paramAddStr(pRsrc, &S[i+1],(j-i-1));
		return 0;
	}
	return -1;
}

/*******************************************************************************
* Function Name  : fineFun
* Description    : 
* Input          : str	-all lowerCase
								 : 
* Output         : None
* Return         : <0			-can not match function name
								 : >=0		-the count of params
*******************************************************************************/
static s8 comply(PARAM_RSRC_T* pRsrc, const u8* STR, CMD_DEF_T* DEF, u8 defLen){
	u8 i,j,k;
	u8 strTmp[PARAM_LEN]={0}, paramStr[PARAM_SUM][PARAM_STR_SZ]={0};
	s16 indxS,indxD,indx,indxFr,indxTo;

	paramReset(pRsrc);
	//copy str to strTmp, lowercase
	for(i=0,j=0; j<PARAM_LEN; i++){
		if(STR[i]==0 || STR[i]=='\r')	break;
		if(STR[i]==' ' || STR[i]=='\t' || STR[i]=='\n' )	continue;
		else if(STR[i]>='A' && STR[i]<='Z')	{	strTmp[j] = STR[i]+32;	j++;}
		else {strTmp[j] = STR[i];	j++; }
	}
	//find '(' ')'
	indxFr = indexOf(strTmp, (const u8*)"(");
	indxTo = lastIndxOf(strTmp, (const u8*)")");
	if( (indxFr <= 0) || (indxTo <= 0) || (indxFr >= indxTo))	return -1;
	//fetch function name
	for( indxS=0,indxD=0; indxS<indxFr; indxS++ ){
		if(indxD >= FUN_NAME_SZ)	break;	
		if(strTmp[indxS]>='0' && strTmp[indxS]<='9')			{	pRsrc->funName[indxD] = strTmp[indxS];	indxD++;	}
		else if(strTmp[indxS]>='a' && strTmp[indxS]<='z')	{	pRsrc->funName[indxD] = strTmp[indxS];	indxD++;	}
		else if(strTmp[indxS]=='.' || strTmp[indxS]=='_' || strTmp[indxS]=='$')	{	pRsrc->funName[indxD] = strTmp[indxS];	indxD++;	}
		else return -3;
	}
	if(indxD==0)	return -4;

	//fetch params string
	i = 0;	j = 0;	k = 0;
	for(indx=indxFr+1; indx <indxTo; indx++){
		if(i>=PARAM_SUM || j>=PARAM_STR_SZ)	return -5;
		if(strTmp[indx] == '"')	k++;
		if(strTmp[indx] == ',' && (k&0x01)==0 )				{	i++;	j=0;	continue;	}
		if(strTmp[indx]<128)	{	paramStr[i][j] = strTmp[indx];	j++;	}
		else return -6;
	}
	//compily params string
	for(i=0; i<PARAM_SUM; i++){
		if(strlen((char*)paramStr[i]) == 0){
			for(j=i+1;j<PARAM_SUM;j++)	if(strlen((char*)paramStr[j]) >0 )	return -22;
			break;
		}
		//apply defs to params stirng
		for(j=0; j<defLen; j++){
			if(strReplace(paramStr[i], PARAM_STR_SZ, DEF[j].KeyStr, DEF[j].ReplaceBy) >= 0 )	break;
		}
		//comply params string
		while(1){
			if(meetStr(pRsrc, paramStr[i]) >=0 )	break;
			else if(meetNum(pRsrc, paramStr[i]) >=0 )	break;
			else return -4;
		}
	}
	return pRsrc->indx;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
