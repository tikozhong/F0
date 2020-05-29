/**
 ******************************************************************************
  * @file    compiler.h
  * @brief
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _COMPILER_H
#define _COMPILER_H

#include "misc.h"
#include "parammeter_DEF.h"

#include "packet.h"

#define COMPLIER_BUFF_SZ 128
#define COMPLIER_PARAM_CNT	10

#define U32	1	
#define S32	2
#define FLT	3
#define STR	4

typedef enum{
	PASS = 0,
	ERR_DEV = -1,
	ERR_NAME = -2,
	ERR_FMT = -3,
	ERR_MEM = -128
} CMPL_RTN;

typedef enum
{
  SHL_CPP    	= 0x00U,
  SHL_C   		= 0x01U,
  SHL_PTH    	= 0x02U,
  SHL_INTEL_LED  = 0x03U
} ShellType_t;

//typedef struct{
//	u8 pool[COMPLIER_BUFF_SZ];	//function name and pool
//	void *params[16];
//	u32 format;
//} PAKET_T;

typedef struct{
	u8 funName[COMPLIER_BUFF_SZ];	//function name and pool
	void *params[COMPLIER_PARAM_CNT];
	u32 format;
	s8 cnt;
	u8 funcNameIndx;
} CompiLer_Rsrc_T;

//typedef struct{
//	CompiLer_Rsrc_T rsrc;
//	//ops
//	s8 (*comply)					(CompiLer_Rsrc_T* pRsrc, const char* S, CMD_DEF_T* DEF, u8 defLen);
//	CMPL_RTN (*isMatchDevName)	(CompiLer_Rsrc_T* pRsrc, const char* DEV);
//	CMPL_RTN (*isMatchFuncName)	(CompiLer_Rsrc_T* pRsrc, const char* FUNC);
//	CMPL_RTN (*isMatchFormat)		(CompiLer_Rsrc_T* pRsrc, ...);
//}CompiLer_Dev_T;

//s8 compilerSetup(CompiLer_Dev_T* pDev);
s8 Compile(const char* S, ShellType_t type, PAKET_T* packet);

s8 cppStyleCompile(const char* S, PAKET_T* packet);
u8 cppStyleIsMatchDev(PAKET_T* packet, const char* DevName);
u8 cppStyleIsMatchFunc(PAKET_T* packet, const char* FuncName);
u8 cppStyleIsMatchArgu(PAKET_T* packet, const char* Format);

s8 split(PAKET_T* packet, const char* S);

#endif
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
