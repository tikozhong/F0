/**
 ******************************************************************************
  * @file    convert.h
  * @brief
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _CONVERT_H
#define _CONVERT_H

#include "misc.h"

typedef struct{
	void (*Uint32ToDecStrN)(char* s, u32 num, u8 nDigi);
	void (*Int32ToDecStrN)(char* s, s32 num, u8 nDigi);
	void (*Uint32ToHexStr)(char* s, u32 num);
	void (*Uint16ToHexStr)(char* s, u16 num);
	void (*Uint8ToHexStr)(char* s, u8 num);
	void (*Uint32ToBinStr)(char* s, u32 num);
	void (*FloatToStr)(char* s, s32 Mul, u32 Div);
	
	void (*Uint32ToStrCatDecN)(char* s, u32 num, u8 nDigi);
	void (*Int32ToStrCatDecN)(char* s, s32 num, u8 nDigi);
	void (*Uint32ToStrCatHex)(char* s, u32 num);
	void (*Uint16ToStrCatHex)(char* s, u16 num);
	void (*Uint8ToStrCatHex)(char* s, u8 num);
	void (*Uint32ToStrCatBin)(char* s, u32 num);
	void (*FloatToStrCat)(char* s, s32 Mul, u32 Div);
		
	s8 (*StrToUint32)(const char* S, u32 *val);
	s8 (*StrToInt32)(const char* S, s32 *val);
	s8 (*StrToFloat)(const char*S, s32* mul , u32* div);

	void 	(*Uint32ToU8Array)	(u32, u8*);
	void 	(*Uint16ToU8Array)	(u16, u8*);
	u32  	(*U8ArrayToUint32)	(u8*);
	u16 	(*U8ArrayToUint16)	(u8*);

} CONVERT_T;

void convertSetup(CONVERT_T *pDev);

#endif
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
