/**
 ******************************************************************************
  * @file    mystring.h
  * @brief
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MY_STRING_H
#define _MY_STRING_H

#include "misc.h"
#include "string.h"

void uppercase ( char* str );	//convert to upperCase
void lowercase ( char* str );	//convert to lowerCase

void strCatX(char* x, const char* S1, const char* S2, const char* S3);
u16 strCpy(char *str0, u16 str0Sz, const char *str1);
u16 strCpyTo(char *str0, u16 str0Sz, const char *str1, u8 symbol);

//s16 strContains(const u8 *pSrc, const u8 *pKey);
s8 strReplace(char *pSrc, u16 pSrcSz, const char *pOld, const char *pNew);
s8 strReplaceBetween(char *pSrc, u16 pSrcSz, const char* HEAD, const char* TAIL, const char *pOld, const char *pNew);
s8 strReplaceBetweenBetween(char *pSrc, u16 pSrcSz, const char* HEAD, const char* TAIL, const char *LineBegin, const char *LineEnd, const char *NewStr);
s8 strReplaceLineBetween(char *pSrc, u16 pSrcSz, const char* HEAD, const char* TAIL, const char *LineBegin, const char *NewStr);
s8 strDefReplace(char *pSrc, u16 pSrcSz, CMD_DEF_T *pDefs, u8 defsCnt);
s8 strInsert(char *pSrc, u16 pSrcSz, u16 indx, const char* str0, const char* str1, const char* str2);

void strMoveLeftN(char* str, u16 n);
void strMoveRightN(char* str, u16 n);

void applyDefine(char *s, u16 sLen, CMD_DEF_T *defs, u8 defsLen);
u8 pointer2str(const char* S, const char* BEGIN_KEY, const char* END_KEY, char *d, u16 dLen);
//u8 pointer2strX(const u8* S, const u8* BEGIN_KEY, const u8* END_KEY, PIECE_T *piece);
//index
s16 indexOfFirstPrntChar(const char *S);	//find first printed symbol index
s16 indexOf(const char *S, const char *KEY);
s16 lastIndxOf(const char *S, const char *KEY);
s16 indexOf_HEAD(const char *S, const char *HEAD, const char *KEY);
s16 indexOf_TAIL(const char *S, const char *TAIL, const char *KEY);
s16 indexOf_Pair(const char *S, const char *HEAD, const char *TAIL, const char *KEY);
s16 indexOf_L(const char *S, u16 len, const char *KEY);

s16 strBetween(const char *pSrc, const char *str0, const char *str1, char *d, u16 dLen);
s16 strBetweenLine(const char *SRC, const char *HEAD, const char *END, const char* BEGIN, char *d, u16 dLen);

u8 isSameStr(const char* S0, const char* S1);
u8 isPair(const char* S0, const char* HEAD, const char* TAIL);

#endif
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
