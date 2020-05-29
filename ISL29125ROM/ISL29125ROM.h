/**********************************************************
filename: ISL29125.h
**********************************************************/
#ifndef _ISL29125_H_
#define _ISL29125_H_

#include "misc.h"
#include "ht2201.h"

#define ADC_BUF_LEN 8
#define R_CH 0
#define G_CH 1
#define B_CH 2
//#define R_LOW_LUX 0
//#define G_LOW_LUX 1
//#define B_LOW_LUX 2
//#define R_HIGH_LUX 3
//#define G_HIGH_LUX 4
//#define B_HIGH_LUX 5
/*****************************************************************************
 @ typedefs
****************************************************************************/
typedef enum {
	R_LOW_RNG 		= 0,
	G_LOW_RNG			= 1,
	B_LOW_RNG			= 2,
	RGB_LOW_RNG		= 3,
	R_HIGH_RNG		= 4,
	G_HIGH_RNG		= 5,
	B_HIGH_RNG		= 6,
	RGB_HIGH_RNG	= 7,
} RGB_SENSOR_MODE;

typedef enum {
	RAW 				= 0,
	CAL					= 1,
	CROSS				= 2,
	CAL_CROSS		= 3,
} RGB_OUTPUT_METHOD;

typedef enum {
	ISL291259_PWR_DWN = 0,
	ISL291259_G				= 1,
	ISL291259_R				= 2,
	ISL291259_B				= 3,
	ISL291259_STBY		= 4,
	ISL291259_RGB			= 5,
	ISL291259_RG			= 6,
	ISL291259_GB			= 7,
} ISL291259_MODE;

typedef enum {
	ISL291259_LOW  = 0<<3,
	ISL291259_HIGH = 1<<3,
} ISL291259_SENSITIVITY;

typedef struct{
	CAL32_T RL,GL,BL;
	CAL32_T RH,GH,BH;
	u8 checkCode;
} ISL291259_CAL_T;

typedef struct{
	char name[DEV_NAME_LEN];
	/* config	*/
	PIN_T INT;
	IIC_IO_Dev_T DevIIC;
	HT2201Dev_t ht2201;
	//REG
	u8 ID;
	u16 adcR[ADC_BUF_LEN];
	u16 adcG[ADC_BUF_LEN];
	u16 adcB[ADC_BUF_LEN];
	u16 outRd;
	u16 outGn;
	u16 outBl;
	RGB_OUTPUT_METHOD outMethod;
	__IO u16 cvrtTimes;
	u16 cvrtIndx;
	ISL291259_CAL_T calCfg;	//CALIBRATION
	RGB_SENSOR_MODE curMode;
	u8 msgEn;
	char callBackMsg[CALLBACK_MSG_LEN];
	//delegrate
	void (*SendNewMsg)	(const char* MSG);
	void (*DelayMs)			(u8 ms);
}ISL291259_Rsrc_T;

typedef struct{
	ISL291259_Rsrc_T rsrc;
	void (*Reset)								(ISL291259_Rsrc_T* pRsrc);
//	STATUS_ISL291259 (*Config)	(ISL291259_Rsrc_T *pRsrc, ISL291259_MODE mode, ISL291259_SENSITIVITY sns, ISL291259_RESOLUTION res);
	DEV_STATUS (*Polling)				(ISL291259_Rsrc_T* pRsrc);
	DEV_STATUS (*StartConvert)	(ISL291259_Rsrc_T *pRsrc, RGB_SENSOR_MODE mode, u16 times);
	DEV_STATUS (*StopConvert)		(ISL291259_Rsrc_T *pRsrc);
	DEV_STATUS (*OffsetCal)			(ISL291259_Rsrc_T *pRsrc, RGB_SENSOR_MODE mode);
	DEV_STATUS (*GainCal)				(ISL291259_Rsrc_T *pRsrc, RGB_SENSOR_MODE mode, u16 trueVal);
	DEV_STATUS (*ResetCal)			(ISL291259_Rsrc_T *pRsrc, RGB_SENSOR_MODE mode, CAL32_T cal);
	void (*ResetCallBackMsg)		(ISL291259_Rsrc_T *pRsrc);
	u32 errFlags;
}ISL291259_Dev_T;
//sizeof(ISL291259_Dev_T) = 232

//typedef struct{	
//	const u8* NAME;
//	PIN_T scl; 
//	PIN_T sda; 
//	PIN_T irq; 
//	u8 msgEn;
//} ISL291259_SETUP_PARAM_T;

s8 isl29125DevSetup(
	ISL291259_Dev_T *pDev, 
	const DEV_PORT* DevPort,
	void (*SendNewMsg)	(const char* MSG) 
);

#endif
