/**********************************************************
filename: dictionary.h
**********************************************************/
#ifndef _DICT_H_
#define _DICT_H_

#include "misc.h"

#define DICT_LINE_SZ	(32)
//return def
#define DICT_RTN_PASS			(0)		//pass
#define DICT_RTN_CHK_FAIL		(-1)	//CRC校验错误
#define DICT_RTN_NO_SEG			(-2)	//不存在的词语
#define DICT_RTN_OVER1			(-3)	//空间不够了

/*****************************************************************************
 @ structure
****************************************************************************/
//=======================================================
//lineNN	|[CHK 1X]	[CYC 3X]	[CONTENT 24]	[SEG 4X]|		//32 bytes per line
//line..	|[CHK 1X]	[CYC 3X]	[CONTENT 24]	[SEG 4X]|		//32 bytes per line
//line01	|[CHK 1X]	[CYC 3X]	[CONTENT 24]	[SEG 4X]|		//32 bytes per line
//line00	|[CHK 1X]	[CYC 3X]	[CONTENT 24]	[SEG 4X]|		//32 bytes per line
//				|31																			0	|
//=======================================================			

/*****************************************************************************
 @ typedefs
****************************************************************************/
typedef struct{
	char name[DEV_NAME_LEN];
	u32 romStartAddr;
	u16 romLen;
	u32 cycTimesUpper;	//cycle times	
	s8 (*ioWrite)(u16 addr, const u8 *pDat, u16 nBytes);
	s8 (*ioRead)(u16 addr, u8 *pDat, u16 nBytes);	
	void (*printOut)(const char* DEV, const char* FUNC, const char* KEY, const char* VAL);
	u16 lines;					//lines = romLen/DICT_PAGE_SZ
}DictRsrc_t;

typedef struct{
	DictRsrc_t rsrc;
	//basic
	s8 (*WriteStr)	(DictRsrc_t* pRsrc, const char* segName, const char* content, u8 len);
	s8 (*ReadStr)		(DictRsrc_t* pRsrc, const char* segName, char* content, u8 len, u32* cycTz);
	void (*ReadStrOut)		(DictRsrc_t* pRsrc);
//	s8 (*WriteNum)	(DictRsrc_t* pRsrc, const char* segName, s32 num);
//	s8 (*ReadNum)		(DictRsrc_t* pRsrc, const char* segName, s32* num);
	s8 (*Remove)		(DictRsrc_t* pRsrc, const char* segName);
	s8 (*RemoveAll)	(DictRsrc_t* pRsrc);
//	u32 (*GetCycle)	(DictRsrc_t* pRsrc, const char* segName);
	u16 (*Verify)		(DictRsrc_t* pRsrc);
	u8 (*FetchLine)	(DictRsrc_t* pRsrc, u16 lineIndx, char* key, char* val);
}DictDev_T;

void DictSetup(
	DictDev_T *pDev, 
	u32 romStartAddr, 
	u16 romLen, 
	u32 cycTimes,
	s8 (*ioWrite)(u16 addr, const u8 *pDat, u16 nBytes),
	s8 (*ioRead)(u16 addr, u8 *pDat, u16 nBytes)
);
#endif
