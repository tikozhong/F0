#ifndef _CYLINDER_H
#define _CYLINDER_H

/**********************************************
filename:	cylinder.h
**********************************************/
#include "misc.h"
//#include "input.h"
//#include "output.h"
/**********************************************
 @ public defines
**********************************************/
#define CLDR_TIMEOUT 0
#define CLDR_GOTTO   1
#define NULL_INDX 0XFF

/**********************************************
 @ typedefs
**********************************************/
typedef enum {
	POS_MOVING 	= 0,	//
	POS_RESET		= 1,	//
	POS_ACTION	= 2		//
}cldrPos_T;

typedef enum {
	CLDR_L = 0,
	CLDR_H = 1
} cldrLogic_t;

typedef struct{
	u8 INDX;
	cldrLogic_t resetPosLogic;
} cldrPin_T;

typedef struct{
	char name[DEV_NAME_LEN];
	/* sensor config  	*/	
	u32 inputStatusCpy;
	cldrPin_T sensorReset;
	cldrPin_T sensorAction;	
	/* driver config  	*/	
	cldrPin_T drvReset;
	cldrPin_T drvAction;
	/* control  */
	cldrPos_T tgtPos;			/* target position 		*/
	cldrPos_T curPos;			/* current position,read only	*/
//	void (*newPosCallback)	(const u8 *devName, cldrPos_T curPos);
//	void (*timeOutCallback)	(const u8 *devName);
	u16 tick;
	u8 tickUnit;	//in ms
	u16 timeout;	//in ms
	u8 msgEn;
	char callBackMsg[CALLBACK_MSG_LEN];
}CLDR_RSRC_T;

typedef struct{
	CLDR_RSRC_T rsrc;
	void (*TickTask)(CLDR_RSRC_T* pRsrc, u32 inputStatus, u32 *outputStatus);
	void (*GotoPos)(CLDR_RSRC_T* pRsrc, cldrPos_T pos, u16 timeout);
	void (*MakeCallBackMsg) (CLDR_RSRC_T* pRsrc, char *rtnStr, u16 rtnStrSz);	
	void (*ResetCallBackMsg)(CLDR_RSRC_T* pRsrc);
	u8 (*IsMsg)	(CLDR_RSRC_T* pRsrc);
}CLDR_DEV_T;

DEV_STATUS cylinderSetup(CLDR_DEV_T *pDev, u8 drvRstIndx, u8 drvActIndx, u8 snsRstIndx, u8 snsActIndx, u8 msgEn);

#endif
