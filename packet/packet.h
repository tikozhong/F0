/**
 ******************************************************************************
  * @file    packet.h
  * @brief
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _PACKET_H
#define _PACKET_H

#include "misc.h"

#define PACKE_POOL_SZ 128
#define PACKE_ARG_SZ	16

#define PACKET_DATA_BW	3	//支持的数据类型使用三个位宽来表示
#define PACKET_NULL	0	//emptpy
#define PACKET_S32	1	//%d	有符号整形
#define PACKET_FLT	2	//%f	浮点数
#define PACKET_STR	3	//%s	字符串
#define PACKET_U32	4	//%u	无符号整型
#define PACKET_HEX	5	//%x	无符号16进制
#define PACKET_PTR	6	//%p	字符串指针

typedef enum{
	PAKET_STYLE_ORG = 0,
	PAKET_STYLE_C		= 1,
	PAKET_STYLE_CPP	= 2,
} PAKET_STYLE_T;

typedef struct{
	u8 pool[PACKE_POOL_SZ];	//function name and pool
	void *addr[PACKE_ARG_SZ];
	u32 format[2];
	u8 poolIndx;
	u8 option0;
	u8 style;	
} PAKET_T;

void packetReset(PAKET_T* pPacket);
u8 packetIsMatch(PAKET_T* pPacket, const char* FORMAT);
s8 packetAddTo(PAKET_T* pPacket, const char*FORMAT, ...);
//s8 packetRemoveFr(PAKET_T* pPacket, u8 indx);
void packetSetStyle(PAKET_T* pPacket, PAKET_STYLE_T s);

s16 packetToStrLine(char* str, u8 sz, PAKET_T* p);
s16 packetToStrC(char* str, u8 sz, PAKET_T* p);
s16 packetToStrCpp(char* str, u8 sz, PAKET_T* p);
#endif
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
