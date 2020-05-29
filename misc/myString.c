/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : mystring.c
* Author             : Tiko Zhong
* Date First Issued  : 12/01/2015
* Description        : This file provides a set of functions needed to manage the
*
*******************************************************************************/
#include "mystring.h"
#include "convert.h"

/* using external variables ------------------------------------*/
/* global variables --------------------------------------------*/
/* private variables --------------------------------------------*/
/* private Functions ---------------------------------------------*/
void delSpace ( char* str );	//delete ' '  '\t'  '\r'  '\n'

/*******************************************************************************
* Function Name  : indexOfFirstPrntChar
* Description    : to seek the index of first printed symbol in S.
								 : symbols with ascii value is [32,127] are printed symbol
* Input          : source string
* Output         : 
* Return         : <0		:not found
								 : >=0	:the index of exactly symbol
*******************************************************************************/
s16 indexOfFirstPrntChar(const char *S){
	u16 i;
	for(i=0;1;i++){
		if((S[i]>=32) && (S[i]<=127))	return i;
		else if(S[i]==0)	return -1;
	}
}

s16 indexOf(const char *S, const char *KEY){
	u16 indxS, indxK, sameCnt, keyL;
	
//	if(S==NULL || KEY==NULL)	return -1;	
	
	keyL = strlen((const char*)KEY);
	for(indxS=0; 1; indxS++){
		if(S[indxS] == 0)	break;
		if(S[indxS] != KEY[0])	continue;
		for(indxK=0,sameCnt=0; 1; indxK++){
			if((KEY[indxK] == 0) || (S[indxS+indxK] == 0))	break;
			if(KEY[indxK] == S[indxS+indxK])	sameCnt++;
			else break;
		}
		if(sameCnt == keyL)	return indxS;
	}
	return -1;
}

s16 lastIndxOf(const char *S, const char *KEY){
	u16 indxS, indxK, sameCnt;
	s16 lastIndx = -1;

	for(indxS=0; 1; indxS++){
		if(S[indxS] == 0)	break;
		if(S[indxS] != KEY[0])	continue;
		for(indxK=0,sameCnt=0; 1; indxK++){
			if((KEY[indxK] == 0) || (S[indxS+indxK] == 0))	break;
			if(KEY[indxK] == S[indxS+indxK])	sameCnt++;
			else break;
		}
		if(sameCnt == strlen((const char*)KEY))	lastIndx = indxS;
	}
	return lastIndx;
}


s16 indexOf_HEAD(const char *S, const char *HEAD, const char *KEY){
	return indexOf_Pair(S, HEAD, NULL, KEY);
}

s16 indexOf_TAIL(const char *S, const char *TAIL, const char *KEY){
	return indexOf_Pair(S, NULL, TAIL, KEY);
}

s16 indexOf_Pair(const char *S, const char *HEAD, const char *TAIL, const char *KEY){
	s16 indxHEAD,indxTAIL,indexKEY;
	u16 lenHEAD;
	
	if(KEY == NULL || S == NULL)	return -1;
	if(HEAD == NULL)	indxHEAD = 0;
	else{
		indxHEAD = indexOf(S,HEAD);
		if(indxHEAD < 0)	return -2;
	}
	if(TAIL == NULL)	indxTAIL = strlen((const char*)S)-1;
	else{
		indxTAIL = indexOf(S,TAIL);
		if(indxTAIL < 0)	return -3;
	}
	if(indxTAIL <= indxHEAD)	return -4;
	lenHEAD = strlen((const char*)HEAD);
	indexKEY = indexOf(&S[indxHEAD+lenHEAD],KEY);
	if(indexKEY<0)	return -5;
	indexKEY += indxHEAD+lenHEAD;
	if(indexKEY > indxTAIL)	return -6;
	else {	return indexKEY;	}
}

s16 indexOf_Len(const char *S, u16 len, const char *KEY){
	s16 indexKEY;

	if(KEY == NULL || S == NULL)	return -1;
	indexKEY = indexOf(S,KEY);
	if(indexKEY < 0)	return -2;
	else if(indexKEY >= len)	return -3;
	else{ return indexKEY;	}
}

s16 indexOf_L(const char *S, u16 len, const char *KEY){
	u16 indxS, indxK, sameCnt, keyL;
	
	if(KEY == NULL || S == NULL)	return -1;
	keyL = strlen((const char*)KEY);
	
	for(indxS=0; indxS<len; indxS++){
		if(S[indxS] != KEY[0])	continue;
		for(indxK=0,sameCnt=0; 1; indxK++){
			if((KEY[indxK] == 0) || (S[indxS+indxK] == 0))	break;
			if(KEY[indxK] == S[indxS+indxK]){	
				sameCnt++;
				if(sameCnt == keyL)	return indxS;
			}
			else break;
		}
		return -2;
	}
	return -1;
}

void applyDefine(char *s, u16 sLen, CMD_DEF_T *defs, u8 defsLen){
	u8 i;
	for(i=0; i<defsLen; i++){
		if(defs[i].KeyStr == NULL || defs[i].ReplaceBy == NULL)	continue;
		if(strReplace(s, sLen, defs[i].KeyStr, defs[i].ReplaceBy) >= 0 )	break;
	}
}

/*******************************************************************************
* Function Name  : complyConstPointer
* Description    : string between BEGIN_KEY and END_KEY is a addr, comply this 
								 : string pointed by this addr and load in d[dLen]
* Input          :
								 : 
* Output         : None
* Return         : counts of params 
*******************************************************************************/
u8 pointer2str(const char* S, const char* BEGIN_KEY, const char* END_KEY, char *d, u16 dLen){
	char s[10]={0};
	u16 i,j,indx0, indx1;
	u32 addrVal;
	const char *p;
	CONVERT_T CONVERT;
	
	convertSetup(&CONVERT);
	if(BEGIN_KEY==NULL || END_KEY==NULL || d==NULL || dLen==0)	return 0;
	
	indx0 = indexOf(S, BEGIN_KEY);
	indx1 = indexOf(S, END_KEY);
	if(indx0 >= indx1 )	return 0;
	//copy begin segment
	for(i=0; i<indx0 && i<dLen; i++){
		if(S[i]==0)	return 0;
		d[i] = S[i];
	}
	//
	j=0;
	for( i=indx0+strlen((char*)BEGIN_KEY); i<indx1 && j<10; i++ )	{
		s[j] = S[i];
		j++;
	}
	//replace string
//	firstNumFrStr (&S[indx0+strlen((char*)BEGIN_KEY)], &addrVal);
	CONVERT.StrToUint32(s, &addrVal);
	p = (const char*)addrVal;	
	strcat(d, p);	
	//copy end segment
	strcat((char*)d, (const char*)&S[indx1+strlen((char*)END_KEY)]);
	return 1;
}

/*******************************************************************************
* Function Name  : charConvert
* Description    : chage upper case to little case
* Input          : - str : pointer to the buffer  containing the string
* Output         : None
* Return         : None
*******************************************************************************/
void uppercase ( char* str )		/*大小写Convert*/
{
    while ( *str != '\0' ){
			if ( ( ( *str ) >= 'a' ) && ( ( *str ) <= 'z' ) ){
				*str = *str - 32;
			}
			str++;
    }
}

void lowercase (char* str)		/*大小写Convert*/
{
    while ( *str != '\0' ){
			if ( ( ( *str ) >= 'A' ) && ( ( *str ) <= 'Z' ) ){
				*str = *str + 32;
			}
			str++;
    }
}

/*******************************************************************************
* Function Name  : delSpace
* Description    : 
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void delSpace(char *str){
	u16 len;
	len = strlen((char*)str);
	if(len == 0)	return;
	while(strReplace(str, len, " ", "") >= 0){}
	while(strReplace(str, len, "\t", "")>=0){}
	while(strReplace(str, len, "\n", "")>=0){}
	while(strReplace(str, len, "\r", "")>=0){}
}

/*******************************************************************************
* Function Name  : strMoveLeftN
* Description    : string loop left
* Input          : - str : pointer to the buffer  containing the string
* Output         : None
* Return         : None
*******************************************************************************/
void strMoveLeftN(char* str, u16 n){
	u16 i,len;
	if(n==0)	return;
	len = strlen((char*)str);
	for(i=0; i<len-n; i++)	str[i] = str[i+n];
	for(i=0; i<n; i++)	str[len-1-i] = 0;
}

/*******************************************************************************
* Function Name  : strMoveRightN
* Description    : str move right n by bytes
* Input          : - str : pointer to the buffer  containing the string
* Output         : None
* Return         : None
*******************************************************************************/
void strMoveRightN(char* str, u16 n){
	u16 i,len;
	if(n==0)	return;
	len = strlen((char*)str);
	for(i=0; i<len; i++)	str[len-1+n-i] = str[len-1-i];
//	for(i=0; i<n; i++)	str[i] = 0;
}

u8 isSameStr(const char* S0, const char* S1){
	u16 len0, len1, len;
	len0 = strlen((const char*)S0);
	len1 = strlen((const char*)S1);
	if(len0==len1 && len0 && len1){
		for(len=0; len<len0; len++)
			if(S0[len] != S1[len])	return 0;
		return 1;
	}
	return 0;
}

u8 isPair(const char* S0, const char* HEAD, const char* TAIL){
	s16 indxHEAD,indxTAIL;
	u16 lenHEAD;
	
	if(S0 == NULL)	return 0;
	
	if(HEAD==NULL)	indxHEAD = 0;
	else{
		indxHEAD = indexOf(S0, HEAD);
		if(indxHEAD < 0)	return 0;
	}
	
	if(TAIL==NULL)	return 1;
	else{
		lenHEAD = strlen((const char*)HEAD);
		indxTAIL = indexOf(&S0[indxHEAD+lenHEAD], TAIL);
		if(indxTAIL < 0)	return 0;
		else return 1;
	}
}

/******************************************************
* Function Name  : strReplace
* Description    : pSrc = (pStr=>pSel, pStr替换掉pSel)
* Input          : pSrc			-the source text	
									 pSrcLen	-the length of pSrc
									 pSel			-the text be replaced
									 pStr			-Using pStr to replace pSel in pSrc
* Output         : 
* Return         : new char count of d
******************************************************/
s8 strReplace(char *pSrc, u16 pSrcSz, const char *pOld, const char *pNew){
	u16 pOldLen,pNewLen,pSrcLen,i;
	s16 indx;
	//if no found pSel in pSrc
	indx = indexOf(pSrc, pOld);	if( indx < 0 )	return -1;
	//if length of pSrc is no enough
	pSrcLen = strlen((const char*)pSrc);
	pNewLen = strlen((const char*)pNew);
	pOldLen = strlen((const char*)pOld);	if(pOldLen == 0)	return -2;

	if(pSrcSz < pSrcLen+pNewLen-pOldLen)	return -3;
	//to make space for pStr
	if(pNewLen > pOldLen)	strMoveRightN(&pSrc[indx], (pNewLen-pOldLen));
	else if(pNewLen < pOldLen)	strMoveLeftN(&pSrc[indx], (pOldLen-pNewLen));
	else{ }
	//copy data
	for(i=0;i<pNewLen;i++)	pSrc[indx+i] = pNew[i];
	return i;
}

s8 strReplaceBetween(char *pSrc, u16 pSrcSz, const char* HEAD, const char* TAIL, const char *pOld, const char *pNew){
	u16 pOldLen,pNewLen,pSrcLen,i;
	s16 indx,indx0,indx1;
	
	//if no found pSel in pSrc	
	indx0 = indexOf(pSrc, HEAD);	if( indx0 < 0 )	return -1;
	indx1 = indexOf(pSrc, TAIL);	if( indx1 < 0 )	return -1;
	indx = indexOf(&pSrc[indx0], pOld);	if( indx < 0 )	return -1;
	indx += indx0;
	
	//if length of pSrc is no enough
	pSrcLen = strlen((const char*)pSrc);
	pNewLen = strlen((const char*)pNew);
	pOldLen = strlen((const char*)pOld);	if(pOldLen == 0)	return -2;

	if(pSrcSz < pSrcLen+pNewLen-pOldLen)	return -3;
	//to make space for pStr
	if(pNewLen > pOldLen)	strMoveRightN(&pSrc[indx], (pNewLen-pOldLen));
	else if(pNewLen < pOldLen)	strMoveLeftN(&pSrc[indx], (pOldLen-pNewLen));
	else{ }
	//copy data
	for(i=0;i<pNewLen;i++)	pSrc[indx+i] = pNew[i];
	return i;
}

s8 strReplaceBetweenBetween(char *pSrc, u16 pSrcSz, const char* HEAD, const char* TAIL, const char *Begin, const char *End, const char *NewStr){
	u16 pOldLen,pNewLen,pSrcLen,i;
	s16 indx0,indx1,indx2,indx3;
	
	//if no found pSel in pSrc	
	if(HEAD == NULL)	indx0 = 0;
	else{	indx0 = indexOf(pSrc, HEAD);	if( indx0 < 0 )	return -1;	}
	
	if(TAIL == NULL)	indx1 = strlen((const char*)pSrc)-1;
	else	{ indx1 = indexOf(pSrc, TAIL);	if( indx1 < 0 )	return -2;	}
	
	i = strlen((const char*)HEAD);
	indx2 = indexOf(&pSrc[indx0+i], Begin);	if( indx2 < 0 )	return -3;
	indx2 += indx0+i;
	
	i = strlen((const char*)Begin);
	indx3 = indexOf(&pSrc[indx2+i], End);	if( indx3 < 0 )	return -4;
	indx3 += indx2+i;

	if(indx0>indx2 || indx2>indx3 || indx3>indx1) return -5;

	//if length of pSrc is no enough
	pSrcLen = strlen((const char*)pSrc);
	pNewLen = strlen((const char*)NewStr);
	pOldLen = (indx3-indx2)+strlen((const char*)End);	if(pOldLen == 0)	return -6;

	if(pSrcSz < pSrcLen+pNewLen-pOldLen)	return -7;
	//to make space for pStr
	if(pNewLen > pOldLen)	strMoveRightN(&pSrc[indx2], (pNewLen-pOldLen));
	else if(pNewLen < pOldLen)	strMoveLeftN(&pSrc[indx2], (pOldLen-pNewLen));
	else{}
	//copy data
	for(i=0;i<pNewLen;i++)	pSrc[indx2+i] = NewStr[i];
	return i;
}

s8 strReplaceLineBetween(char *pSrc, u16 pSrcSz, const char* HEAD, const char* TAIL, const char *LineBegin, const char *NewStr){
	u16 pOldLen,pNewLen,pSrcLen,i;
	s16 indx0,indx1,indx2,indx3;
	
	//if no found HEAD in pSrc	
	if(HEAD == NULL)	indx0 = 0;
	else{	indx0 = indexOf(pSrc, HEAD);	if( indx0 < 0 )	return -1;	}
	
	//if no found TAIL in pSrc
	if(TAIL == NULL)	indx1 = strlen((const char*)pSrc)-1;
	else	{ indx1 = indexOf(pSrc, TAIL);	if( indx1 < 0 )	return -2;	}
	
	i = strlen((const char*)HEAD);
	indx2 = indexOf(&pSrc[indx0+i], LineBegin);	if( indx2 < 0 )	return -3;
	indx2 += indx0+i;
	
	i = strlen((const char*)LineBegin);
	indx3 = indexOf(&pSrc[indx2+i], "\r\n");	if( indx3 < 0 )	return -4;
	indx3 += indx2+i;

	if(indx0>indx2 || indx2>indx3 || indx3>indx1) return -5;

	//if length of pSrc is no enough
	pSrcLen = strlen((const char*)pSrc);
	pNewLen = strlen((const char*)NewStr);
	pOldLen = (indx3-indx2);	if(pOldLen == 0)	return -6;

	if(pSrcSz < pSrcLen+pNewLen-pOldLen)	return -7;
	//to make space for pStr
	if(pNewLen > pOldLen)	strMoveRightN(&pSrc[indx2], (pNewLen-pOldLen));
	else if(pNewLen < pOldLen)	strMoveLeftN(&pSrc[indx2], (pOldLen-pNewLen));
	else{}
	//copy data
	for(i=0;i<pNewLen;i++)	pSrc[indx2+i] = NewStr[i];
	return i;
}

s8 strDefReplace(char *pSrc, u16 srcSz, CMD_DEF_T *pDefs, u8 defsCnt){
	s16 indx;
	u8 i;
	//start with "("
	indx = indexOf(pSrc, "(");
	if(indx < 0) 	return indx;
	//replace DEFS with val
	for(i=0;i<defsCnt;i++)	{
		strReplace(&pSrc[indx], srcSz, pDefs[i].KeyStr, pDefs[i].ReplaceBy);
	}
	return 0;
}

s8 strInsert(char *pSrc, u16 pSrcSz, u16 indx, const char* str0, const char* str1, const char* str2){
	u16 i,len0,len1,len2,lenS;

	len0 = strlen((const char*)str0);	
	len1 = strlen((const char*)str1);	
	len2 = strlen((const char*)str2);	
	lenS = strlen((char*)pSrc);	
	if((len0+len1+len2+lenS) > pSrcSz)	return -1;
	strMoveRightN(&pSrc[indx], (len0+len1+len2));
	for(i=0;i<len0;i++)	pSrc[indx+i] = str0[i];
	for(i=0;i<len1;i++)	pSrc[indx+len0+i] = str1[i];
	for(i=0;i<len2;i++)	pSrc[indx+len0+len1+i] = str2[i];
	return 0;
}

s16 _strBetween(const char *pSrc, const char *str0, const char *str1, char *d, u16 dLen){
	u16 i;	s16 indx0=0, indx1=0;

	indx0 = indexOf(pSrc, str0); 
	if(indx0 < 0 )	return -1;

	indx1 = indexOf(pSrc, str1);
	if(indx1 < 0)	return -2;
	
	indx0 += strlen((const char*)str0);
	if(indx0 >= indx1)	return -3;

	memset(d,0,dLen);
	for(i=0; i<dLen && i<(indx1-indx0); i++)	d[i] = pSrc[indx0+i];
	return indx0;
}

s16 strBetween(const char *pSrc, const char *str0, const char *str1, char *d, u16 dLen){
	u16 i;	s16 indx0=0, indx1=0;

	if(strlen((const char*)str0)==0 || str0==NULL){	indx0 = 0; }
	else{
		indx0 = indexOf(pSrc, str0); 
		if(indx0 < 0 )	return -1;		
	}

	if(strlen((const char*)str1)==0 || str1==NULL){	indx1 = strlen((const char*)pSrc) -1;}
	else{
		indx1 = indexOf(pSrc, str1);
		if(indx1 < 0)	return -2;
	}
	
	indx0 += strlen((const char*)str0);
	if(indx0 >= indx1)	return -3;
	
	//here, it has some improved
//	memset(d,0,dLen);
//	for(i=0; i<dLen && i<(indx1-indx0); i++)	d[i] = pSrc[indx0+i];

	for(i=0; i<dLen; i++){
		if(i<(indx1-indx0)){
			d[i] = pSrc[indx0+i];
		}
		else	d[i] = 0;
	}
	
	return indx0;
}

s16 strBetweenLine(const char *SRC, const char *HEAD, const char *TAIL, const char* BEGIN, char *d, u16 dLen){
	u16 i;	s16 indx0=0, indx1=0, indx2 = 0;

	if(strlen((const char*)HEAD)==0 || HEAD == NULL){	indx0 = 0;	}
	else{
		indx0 = indexOf(SRC, HEAD);
		if(indx0 < 0 )	return -1;		
	}
	
	if(strlen((const char*)TAIL)==0 || TAIL==NULL){	indx1 = strlen((const char*)SRC)-1;	}
	else{
		indx1 = indexOf(SRC, TAIL);
		if(indx1 < 0)	return -2;
	}
	
	indx0 += strlen((const char*)HEAD);
	if(indx0 >= indx1)	return -3;

	indx2 = indexOf(&SRC[indx0],BEGIN);
	if(indx2 < 0)	return -4;
	
	indx2 += indx0;
	if(indx2 >= indx1)	return -5;

	memset(d,0,dLen);
	for(i=0; i<dLen && i<(indx1-indx2); i++){
		if(SRC[indx2+i] == '\r' || SRC[indx2+i] == '\n' )	break;
		d[i] = SRC[indx2+i];
	}

	return indx2;
}

/*******************************************************************************
* Function Name  : strCat
* Description    : d = d+s
* Input          :
* Output         : 
* Return         : new char count of d
*******************************************************************************/
void strCatX(char*x, const char* S1, const char* S2, const char* S3){
	if(S1)	strcat((char*)x, (const char*)S1);
	if(S2)	strcat((char*)x, (const char*)S2);
	if(S3)	strcat((char*)x, (const char*)S3);
}

/*******************************************************************************
* Function Name  : strCat
* Description    : d = d+s
* Input          :
* Output         : 
* Return         : new char count of d
*******************************************************************************/
u16 strCpy(char *str0, u16 str0Sz, const char *str1){
	u16 i;
	
	if(str0Sz > 0){
		memset(str0,0,str0Sz);
		for(i=0;i<str0Sz;i++){
			if(str1[i] == 0)	break;
			str0[i] = str1[i];
		}	
	}
	else{
		for(i=0;1;i++){
			if(str1[i]==0)	break;
			str0[i] = str1[i];
		}
	}
	return i;
}

u16 strCpyTo(char *str0, u16 str0Sz, const char *str1, u8 symbol){
	u16 i;
	if(str0Sz != 0){
		memset(str0, 0, str0Sz);
		for(i=0;i<str0Sz;i++){
			if(str1[i]==0 || str1[i]==symbol)	break;
			str0[i] = str1[i];
		}	
	}
	else{
		for(i=0;1;i++){
			if(str1[i]==0 || str1[i]==symbol)	break;
			str0[i] = str1[i];
		}
	}
	return i;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
