/**********************************************
filename:	cylinders.c
**********************************************/
#include "board.h"
#include "publicRsrc.h"
#include "input.h"
#include "output.h"
#include "cylinder.h"

#define RUN_TASK_PER_MS	16

//extern INPUT_DEV_T devInput;		//should be defined somewhere else
//extern OUTPUT_DEV_T devOutput;	//should be defined somewhere else

void cylinderTick(CLDR_RSRC_T* pRsrc, u32 inputStatus, u32 *outputStatus);
void cylinderGoto(CLDR_RSRC_T* pRsrc, cldrPos_T pos, u16 timeout);
void cylinderResetCallBackMsg(CLDR_RSRC_T* pRsrc);
u8 isMsgAvailable_Cldr(CLDR_RSRC_T* pRsrc);
void makeCallbackMsg_Cldr(CLDR_RSRC_T* pRsrc, char *rtnStr, u16 rtnStrSz);

DEV_STATUS cylinderSetup(CLDR_DEV_T *pDev, u8 drvRstIndx, u8 drvActIndx, u8 snsRstIndx, u8 snsActIndx, u8 msgEn){
	CLDR_RSRC_T *pRsrc = &pDev->rsrc;;
	//resource
	pRsrc->sensorReset.INDX = snsRstIndx;	
	pRsrc->sensorReset.resetPosLogic = CLDR_L;	//Low while in reset position
	pRsrc->sensorAction.INDX = snsActIndx;
	pRsrc->sensorAction.resetPosLogic = CLDR_H;	//High while in reset position
	pRsrc->drvReset.INDX = drvRstIndx;
	pRsrc->drvReset.resetPosLogic = CLDR_L;			//drive low while drive reset position
	pRsrc->drvAction.INDX = drvActIndx;
	pRsrc->drvAction.resetPosLogic = CLDR_H;		//drive high while drive reset position
	pRsrc->curPos = POS_MOVING;
	pRsrc->tgtPos = POS_RESET;
	pRsrc->tick = 0;
	pRsrc->tickUnit = RUN_TASK_PER_MS;		//in 16ms tick polling
	pRsrc->timeout = 5000;								//5000ms Timeout
	pRsrc->msgEn = msgEn;
	memset(pRsrc->callBackMsg, 0, CALLBACK_MSG_LEN);
	//ops
	pDev->TickTask = cylinderTick;
	pDev->GotoPos = cylinderGoto;
	pDev->IsMsg = isMsgAvailable_Cldr;
	pDev->MakeCallBackMsg = makeCallbackMsg_Cldr;
	pDev->ResetCallBackMsg = cylinderResetCallBackMsg;
	return DEV_SUCCESS;
}

void cylinderResetCallBackMsg(CLDR_RSRC_T* pRsrc){
	u8 i;
	for(i=0;i<CALLBACK_MSG_LEN;i++)	
		pRsrc->callBackMsg[i] = 0;
}

/*******************************************************************************
* Function Name  : cylinderTick
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void cylinderTick(CLDR_RSRC_T* pRsrc, u32 inputStatus, u32 *outputStatus){
	cldrPos_T pos = POS_MOVING;
	cldrLogic_t resetSnsLogic = CLDR_L, actionSnsLogic = CLDR_L;
	
	/* update curPosition */
	if(pRsrc->sensorReset.INDX == NULL_INDX){	strCpy(pRsrc->callBackMsg, CALLBACK_MSG_LEN, "illegal_sensor_reset_indx");	}
	else if(pRsrc->sensorAction.INDX == NULL_INDX) {
		if((inputStatus & (u32)1<<pRsrc->sensorReset.INDX) != 0)		resetSnsLogic = CLDR_H;
		if(resetSnsLogic==pRsrc->sensorReset.resetPosLogic)	pos = POS_RESET;
		if(resetSnsLogic!=pRsrc->sensorReset.resetPosLogic)	pos = POS_ACTION;
	}
	else{	
		if((inputStatus & (u32)1<<pRsrc->sensorReset.INDX) != 0)		resetSnsLogic = CLDR_H;
		if((inputStatus & (u32)1<<pRsrc->sensorAction.INDX) != 0)		actionSnsLogic = CLDR_H;
		if(resetSnsLogic==pRsrc->sensorReset.resetPosLogic && actionSnsLogic==pRsrc->sensorAction.resetPosLogic )
			pos = POS_RESET;
		if(resetSnsLogic!=pRsrc->sensorReset.resetPosLogic && actionSnsLogic!=pRsrc->sensorAction.resetPosLogic )
			pos = POS_ACTION;
	}
	if(pRsrc->curPos != pos){
		if(pos==POS_RESET)	strCpy(pRsrc->callBackMsg, CALLBACK_MSG_LEN, "arrive_reset");
		if(pos==POS_ACTION)	strCpy(pRsrc->callBackMsg, CALLBACK_MSG_LEN, "arrive_action");
		pRsrc->curPos = pos;
	}
	/* drive to position */
	if(pRsrc->curPos != pRsrc->tgtPos){
		if(pRsrc->tgtPos == POS_ACTION){
			if(pRsrc->drvReset.INDX < 32){
				if(pRsrc->drvReset.resetPosLogic == CLDR_L)	*outputStatus |= (u32)1<<pRsrc->drvReset.INDX;
				else	*outputStatus &= ((u32)1<<pRsrc->drvReset.INDX)^0xffffffff;	
			}
			if(pRsrc->drvAction.INDX < 32){
				if(pRsrc->drvAction.resetPosLogic == CLDR_L)	*outputStatus |= (u32)1<<pRsrc->drvAction.INDX;	
				else	*outputStatus &= ((u32)1<<pRsrc->drvAction.INDX)^0xffffffff;	
			}
		}
		else{
			if(pRsrc->drvReset.INDX < 32){
				if(pRsrc->drvReset.resetPosLogic == CLDR_L)	*outputStatus &= ((u32)1<<pRsrc->drvReset.INDX)^0xffffffff;	
				else	*outputStatus |= (u32)1<<pRsrc->drvReset.INDX;	
			}
			if(pRsrc->drvAction.INDX < 32){
				if(pRsrc->drvAction.resetPosLogic == CLDR_L)	*outputStatus &= ((u32)1<<pRsrc->drvAction.INDX)^0xffffffff;	
				else	*outputStatus |= (u32)1<<pRsrc->drvAction.INDX;	
			}
		}
	}
	pRsrc->tick++;
	if((pRsrc->tick*pRsrc->tickUnit) >= pRsrc->timeout)	{
		strCpy(pRsrc->callBackMsg, CALLBACK_MSG_LEN, "timeout");
		pRsrc->tick = 0;
	}
}

/*******************************************************************************
* Function Name  : cylinderGotoUntil
* Description    : 
* Input          : None
* Output         : None
* Return         : CYLDR_TIMEOUT: timeout		CYLDR_GOTTO:reach pos within timeout
*******************************************************************************/
void cylinderGoto(CLDR_RSRC_T* pRsrc, cldrPos_T pos, u16 timeout){
	pRsrc->tgtPos = pos;
	pRsrc->tick = 0;
	pRsrc->timeout = timeout;
}

u8 isMsgAvailable_Cldr(CLDR_RSRC_T* pRsrc){
	if(pRsrc->msgEn == 0)	return 0;
	if(strlen((const char*)pRsrc->callBackMsg) > 0)	return 1;
	return 0;
}

void makeCallbackMsg_Cldr(CLDR_RSRC_T* pRsrc, char *rtnStr, u16 rtnStrSz){
	strCpy(rtnStr, rtnStrSz, "+cbm %");
	strcat((char*)rtnStr, (const char*)pRsrc->name);
	strcat((char*)rtnStr, (const char*)".msg= ");
	strcat((char*)rtnStr, (const char*)pRsrc->callBackMsg);
	strcat((char*)rtnStr, (const char*)"\r\n");
	memset(pRsrc->callBackMsg, 0, CALLBACK_MSG_LEN);
}

