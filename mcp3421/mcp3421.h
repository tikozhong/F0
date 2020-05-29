/**********************************************************
filename: MCP3421.h
**********************************************************/
#ifndef _MCP3421_H_
#define _MCP3421_H_

#include "publicRsrc.h"
#include "IIC_IO.h"

#define ADC_BUF_LEN 8
/*****************************************************************************
 @ typedefs
****************************************************************************/
typedef enum {
	SPS240_12BIT = 0,
	SPS060_14BIT = 1,
	SPS015_16BIT = 2,
	SPS004_18BIT = 3,
} MCP3421_Res;

typedef enum {
	X1 = 0,
	X2 = 1,
	X4 = 2,
	X8 = 3,
} MCP3421_Gain;

//typedef struct MCP3421{
//	//RESOURCE
//	u8 name[DEV_NAME_LEN];
//	/* config	*/
//	PIN_T SCL,SDA;
//	MCP3421_Res Res;
//	MCP3421_Gain Gain;
//	
//	CAL32_T Cal;
//	u32 result[ADC_BUF_LEN];
//	__IO u16 cvrtTimes;
//	u8 msgEn;
//	u8 callBackMsg[CALLBACK_MSG_LEN];
//	//delegrate
//	void (*SendNewMsg)	(const u8* MSG);
//	void (*ReadCal)			(const u8* NAME, CAL32_T* pCal);
//	void (*WriteCal)		(const u8* NAME, const CAL32_T CAL);
//	
//	//OPERATIONS
//	DEV_STATUS (*SetMode)				(MCP3421_Rsrc_T* pRsrc, MCP3421_Res res, MCP3421_Gain gain);
//	DEV_STATUS (*Polling)				(MCP3421_Rsrc_T* pRsrc, u8 applyCal);
//	DEV_STATUS (*StartConvert)	(MCP3421_Rsrc_T *pRsrc, u16 times);
//	DEV_STATUS (*StopConvert)		(MCP3421_Rsrc_T *pRsrc);
//	DEV_STATUS (*OffsetCal)			(MCP3421_Rsrc_T *pRsrc);
//	DEV_STATUS (*GainCal)				(MCP3421_Rsrc_T *pRsrc, u32 trueVal);
//	DEV_STATUS (*ResetCal)			(MCP3421_Rsrc_T *pRsrc, CAL32_T cal);
//	void (*ResetCallBackMsg)		(MCP3421_Rsrc_T *pRsrc);
//	u32 errFlags;
//}MCP3421;


typedef struct{
	char name[DEV_NAME_LEN];
	/* config	*/
  IIC_IO_Dev_T iic;
	IIC_IO_Dev_T* pIIC;
//	PIN_T SCL,SDA;
	MCP3421_Res Res;
	MCP3421_Gain Gain;
	u8 tick,tmr,squ;
  u8 devAddr;
	char keyCalOffset[4];	//key in dictionary for offset
	char keyCalMul[4];		//key in dictionary for mul
	char keyCalDiv[4];		//key in dictionary for div
	CAL32_T Cal;
	u32 result[ADC_BUF_LEN];
	__IO u16 cvrtTimes;
//	char callBackMsg[CALLBACK_MSG_LEN];
	u8 error;
	
	//delegrate
	void (*ReadCal)		(const char* TAG, u32* val);
	void (*WriteCal)	(const char* TAG, u32 val);
}MCP3421_Rsrc_T;

typedef struct{
	MCP3421_Rsrc_T rsrc;
	DEV_STATUS (*OneShot)		(MCP3421_Rsrc_T* pRsrc, MCP3421_Res res, MCP3421_Gain gain);
	DEV_STATUS (*Polling)		(MCP3421_Rsrc_T* pRsrc, u8 applyCal);
	DEV_STATUS (*StartConvert)	(MCP3421_Rsrc_T *pRsrc, MCP3421_Res res, MCP3421_Gain gain, u16 times);
	DEV_STATUS (*StopConvert)	(MCP3421_Rsrc_T *pRsrc);
	DEV_STATUS (*OffsetCal)		(MCP3421_Rsrc_T *pRsrc, MCP3421_Res res, MCP3421_Gain gain);
	DEV_STATUS (*GainCal)		(MCP3421_Rsrc_T *pRsrc, MCP3421_Res res, MCP3421_Gain gain, u32 trueVal);
	DEV_STATUS (*ResetCal)		(MCP3421_Rsrc_T *pRsrc, CAL32_T cal);
	u32 errFlags;
}MCP3421_Dev_T;
//sizeof(MCP3421_Dev_T) = 232

s8 mcp3421DevSetup1(	
		MCP3421_Dev_T *pDev, 
		PIN_T SCL, 
		PIN_T SDA, 
    u8 devAddr,
		void (*ReadCal)		(const char* TAG, u32* val),
		void (*WriteCal)	(const char* TAG, u32 val),
		const char* KEY_OFFSET,
		const char* KEY_MUL,
		const char* KEY_DIV
);

s8 mcp3421DevSetup2(	
		MCP3421_Dev_T *pDev, 
		IIC_IO_Dev_T* pIIC,  
    u8 devAddr,
		void (*ReadCal)		(const char* TAG, u32* val),
		void (*WriteCal)	(const char* TAG, u32 val),
		const char* KEY_OFFSET,
		const char* KEY_MUL,
		const char* KEY_DIV
);
		
#endif
